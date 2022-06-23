package projeto;

import java.io.Serializable;
import java.util.ArrayList;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public abstract class Publicacao implements Serializable { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    protected String titulo;
    protected String palavraChave;
    protected int ano, audiencia;
    protected String resumo;
    protected ArrayList<Investigador> autores;

    /**
     * Construtor de objetos do tipo "Publicacao" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     */
    public Publicacao(String titulo, String palavraChave, int ano, int audiencia, String resumo) { //--*--*--*--*--*
        this.titulo = titulo;
        this.palavraChave = palavraChave;
        this.ano = ano;
        this.audiencia = audiencia;
        this.resumo = resumo;
        autores = new ArrayList<>();

    }
    
    /**
     * Devolve "titulo da publicação"
     * @return titulo
     */
    public String getTitulo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return titulo;
    }

    /**
     * Devolve "palavra-chave da publicação"
     * @return palavraChave
     */
    public String getPalavraChave() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return palavraChave;
    }

    /**
     * Devolve "ano de publicação"
     * @return ano
     */
    public int getAno() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return ano;
    }

    /**
     * Devolve "audiência da publicação"
     * @return audiencia
     */
    public int getAudiencia() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return audiencia;
    }

    /**
     * Devolve "resumo da publicação"
     * @return resumo
     */
    public String getResumo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return resumo;
    }
    
    /**
     * Devolve "lista de autores"
     * @return autores
     */
    public ArrayList<Investigador> getAutores() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return autores;
    }
    
    /**
     * Substitui "titulo da publicação"
     * @param titulo  novo "titulo da publicação"
     */
    public void setTitulo(String titulo) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.titulo = titulo;
    }
    
    /**
     * Substitui "palavra-chave da publicação"
     * @param palavraChave nova "palavra-chave da publicação"
     */
    public void setPalavraChave(String palavraChave) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.palavraChave = palavraChave;
    }

    /**
     * Substitui "ano de publicação"
     * @param ano novo "ano de publicação"
     */
    public void setAno(int ano) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.ano = ano;
    }

    /**
     * Substitui "audiencia da publicação"
     * @param audiencia nova "audiencia da publicação"
     */
    public void setAudiencia(int audiencia) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.audiencia = audiencia;
    }

    /**
     * Substitui "resumo da publicação"
     * @param resumo novo "resumo da publicação"
     */
    public void setResumo(String resumo) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.resumo = resumo;
    }

    /**
     * Substitui "lista de autores"
     * @param autores nova "listsa de autores"
     */
    public void setAutores(ArrayList<Investigador> autores) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.autores = autores;
    }

    /**
     * Determina o tipo de publicação
     * @return tipo de publicação em String
     */
    public abstract String tipo(); //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    /**
     * Determina o fator de impacto da publicação
     * @return fator de impacto da publicação em String
     */
    public abstract String calculaFator(); //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    /**
     * Imprime a informação principal de uma publicação
     * @param tabs espaçamento de impressão (estético)
     */
    public void infoPrincipal(String tabs) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        System.out.println(tabs + "Título: " + titulo);
        System.out.print(tabs + "Ano: " + ano + " | ");
        System.out.print("Tipo: " + tipo() + " | ");
        System.out.println("Fator: " + calculaFator());
        System.out.print(tabs + "Autores: ");
        for (int i = 0; i < autores.size(); i++) {
            System.out.println(autores.get(i).nomeAutor());
            System.out.print(tabs + "         "); //estético
        }
        System.out.println(); //estético
    }

    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return titulo + "\n->Resumo: " + resumo + "\n->Palavra-chave: " + palavraChave 
                + "\n->ano: " + ano + "\n->Audiência: " + audiencia;
    }
}
