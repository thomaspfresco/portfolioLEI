CREATE TABLE utilizador (
	status		 BOOL NOT NULL DEFAULT TRUE,
	id	 SERIAL,
	name	 VARCHAR(50) NOT NULL,
	surname	 VARCHAR(50) NOT NULL,
	email	 VARCHAR(62) UNIQUE NOT NULL,
	address	 VARCHAR(512) NOT NULL,
	city	 VARCHAR(512) NOT NULL,
	gender	 CHAR(1) NOT NULL,
	password VARCHAR(512) NOT NULL,
	admin BOOL NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE leilao (
	ean			 NUMERIC(13,0),
	title		 VARCHAR(50) NOT NULL,
	price		 DOUBLE PRECISION NOT NULL,
	endingdate		 TIMESTAMP NOT NULL,
	state		 BOOL NOT NULL DEFAULT TRUE,
	utilizador_id INTEGER NOT NULL,
	PRIMARY KEY(ean)
);

CREATE TABLE licitacao (
	id			 SERIAL,
	bid			 DOUBLE PRECISION NOT NULL,
	date		 TIMESTAMP NOT NULL,
	valid		 BOOL NOT NULL DEFAULT TRUE,
	utilizador_id INTEGER NOT NULL,
	leilao_ean		 NUMERIC(13,0) NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE mensagem (
	id			 SERIAL,
	message		 VARCHAR(512) NOT NULL,
	leilao_ean		 NUMERIC(13,0) NOT NULL,
	utilizador_id INTEGER NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE descricao (
	id		 SERIAL,
	description	 VARCHAR(512) NOT NULL,
	currentdescription BOOL NOT NULL DEFAULT TRUE,
	modifieddate	 TIMESTAMP NOT NULL,
	leilao_ean	 NUMERIC(13,0) NOT NULL,
	PRIMARY KEY(id)
);

CREATE TABLE utilizador_mensagem (
	utilizador_id INTEGER,
	mensagem_id		 INTEGER,
	PRIMARY KEY(utilizador_id, mensagem_id)
);

ALTER TABLE utilizador ADD CONSTRAINT gender_check CHECK (gender = 'M' OR gender = 'F' or gender = 'O');
ALTER TABLE leilao ADD CONSTRAINT ean_not_negative CHECK (ean >= 0);
ALTER TABLE leilao ADD CONSTRAINT leilao_fk1 FOREIGN KEY (utilizador_id) REFERENCES utilizador(id);
ALTER TABLE licitacao ADD CONSTRAINT licitacao_fk1 FOREIGN KEY (utilizador_id) REFERENCES utilizador(id);
ALTER TABLE licitacao ADD CONSTRAINT licitacao_fk2 FOREIGN KEY (leilao_ean) REFERENCES leilao(ean);
ALTER TABLE mensagem ADD CONSTRAINT mensagem_fk1 FOREIGN KEY (leilao_ean) REFERENCES leilao(ean);
ALTER TABLE mensagem ADD CONSTRAINT mensagem_fk2 FOREIGN KEY (utilizador_id) REFERENCES utilizador(id);
ALTER TABLE descricao ADD CONSTRAINT descricao_fk1 FOREIGN KEY (leilao_ean) REFERENCES leilao(ean);
ALTER TABLE utilizador_mensagem ADD CONSTRAINT utilizador_mensagem_fk1 FOREIGN KEY (utilizador_id) REFERENCES utilizador(id);
ALTER TABLE utilizador_mensagem ADD CONSTRAINT utilizador_mensagem_fk2 FOREIGN KEY (mensagem_id) REFERENCES mensagem(id);

CREATE INDEX search_email ON utilizador(email);
CREATE INDEX termino_data ON leilao (endingdate, state);
CREATE INDEX search_owner ON leilao (utilizador_id);
CREATE INDEX which_auction ON licitacao (leilao_ean, bid);
CREATE INDEX bidder ON licitacao (utilizador_id);
CREATE INDEX mural ON mensagem (leilao_ean);
CREATE INDEX atual_descricao ON descricao (leilao_ean, currentdescription);
CREATE INDEX desc_conteudo ON descricao (description);
CREATE INDEX destinies ON utilizador_mensagem(mensagem_id, utilizador_id);


--DESCRIPTION-
create or replace function func_old_description() returns trigger
language plpgsql
as $$
begin
    update descricao
    SET currentdescription = FALSE
    WHERE descricao.leilao_ean = new.leilao_ean AND descricao.currentdescription = TRUE;
    return new;
end;
$$;

create trigger tbi_descricao
before insert on descricao
for each row
execute procedure func_old_description();

--BIGGER BID NOTIFICATION--
create or replace function func_bigger_bid() returns trigger
language plpgsql
as $$
declare
    c1 cursor for
    SELECT MAX(licitacao.bid) FROM licitacao WHERE licitacao.leilao_ean = new.leilao_ean;
    v_bidmax licitacao.bid%type;
    v_destino licitacao.utilizador_id%type;
begin
    open c1;
    fetch c1 into v_bidmax;
    close c1;
    if v_bidmax is not NULL then
        SELECT utilizador_id INTO v_destino
        FROM licitacao
        WHERE licitacao.leilao_ean = new.leilao_ean AND licitacao.bid = v_bidmax;
        INSERT INTO mensagem(message,leilao_ean,utilizador_id)
		VALUES ('Licitacao ultrapassada', new.leilao_ean, new.utilizador_id);
        INSERT INTO utilizador_mensagem(mensagem_id,utilizador_id)
        SELECT MAX(mensagem.id), v_destino
        FROM mensagem;
    end if;
    return new;
end;
$$;

create trigger tbi_licitacao
before insert on licitacao
for each row
execute procedure func_bigger_bid();

--MESSAGE ON AUCTION CREATED--
create or replace function func_first_message() returns trigger
language plpgsql
as $$
begin
    INSERT INTO mensagem(message,leilao_ean,utilizador_id)
    VALUES ('Leilao criado', new.ean, new.utilizador_id);
    return new;
end;
$$;

create trigger tai_leilao
after insert on leilao
for each row
execute procedure func_first_message();


--Funcao para imprimir parte das estatisticas do top 10 leiloes criados
create or replace function estatistica_leiloes_criados() returns TABLE (util_id INTEGER, leiloes_criados BIGINT)
language plpgsql
as $$
begin
    RETURN QUERY
    SELECT utilizador_id, COUNT(ean)
    FROM leilao
    GROUP BY utilizador_id
    ORDER BY 2 DESC
    LIMIT 10;
end;
$$;

--Funcao para imprimir parte das estatisticas do top 10 winners de leiloes
create or replace function estatistica_leiloes_winners() returns TABLE (util_id INTEGER, wiener_counts BIGINT)
language plpgsql
as $$
begin
    RETURN QUERY
    SELECT licitacao.utilizador_id, COUNT(licitacao.utilizador_id)
    FROM licitacao, leilao
    WHERE leilao.ean = licitacao.leilao_ean AND licitacao.bid = leilao.price
    GROUP BY licitacao.utilizador_id
    ORDER BY 2 DESC
    LIMIT 10;
end;
$$;

--Funcao para imprimir parte das estatisticas de numero de estatisticas de a 10 dias
create or replace function estatistica_ten_days() returns INTEGER
language plpgsql
as $$
declare
    retorna INTEGER;
begin
    SELECT COUNT(*) INTO retorna
    FROM leilao
    WHERE EXTRACT(DAYS FROM (CURRENT_TIMESTAMP-endingdate)) <= 10;
    return retorna;
end;
$$;

--funcao para eliminar as licitacoes acimas do utilizador banido
create or replace function kill_user(util INTEGER) returns INTEGER
language plpgsql
as $$
declare
    c1 cursor for
    SELECT MIN(bid), leilao_ean
    FROM licitacao
    WHERE licitacao.utilizador_id = util
    GROUP BY leilao_ean;
    v_leilao_temp licitacao.leilao_ean%type;
    v_bid_temp licitacao.bid%type;
begin
    open c1;
    loop
        fetch c1 into v_bid_temp, v_leilao_temp;
        exit when not found;
        UPDATE licitacao
        SET valid = FALSE
        WHERE leilao_ean = v_leilao_temp AND bid >= v_bid_temp;
        UPDATE licitacao
        SET valid = TRUE, bid = v_bid_temp
        WHERE bid IN (SELECT MAX(bid) FROM licitacao WHERE leilao_ean = v_leilao_temp AND valid = FALSE AND utilizador_id <> util) AND leilao_ean = v_leilao_temp;
		UPDATE leilao
        SET price = v_bid_temp
        WHERE ean = v_leilao_temp;
    end loop;
    close c1;
    return 0;
end;
$$;

--FIM---