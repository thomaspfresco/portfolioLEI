
package projeto;

import java.io.Serializable;
import java.util.ArrayList;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public abstract class Investigador implements Serializable { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    protected String nome;
    protected String email;
    protected Grupo grupo;
    protected ArrayList<Publicacao> publicacoes;
    
    /**
     * Construtor de objetos do tipo "Investigador" com parâmetros string
     * @param nome nome do investigador
     * @param email e-mail do investigador
     */
    public Investigador(String nome,String email) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
        this.email = email;
        publicacoes = new ArrayList<>();
    }

    /**
     * Devolve "nome"
     * @return nome
     */
    public String getNome() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome;
    }

    /**
     * Devolve "e-mail"
     * @return email
     */
    public String getEmail() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return email;
    }

    /**
     * Devolve "grupo de investigação"
     * @return grupo
     */
    public Grupo getGrupo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return grupo;
    }

    /**
     * Devolve "lista de publicações"
     * @return publicacaoes
     */
    public ArrayList<Publicacao> getPublicacoes() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return publicacoes;
    }

    /**
     * Substitui "nome"
     * @param nome novo "nome"
     */
    public void setNome(String nome) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
    }

    /**
     * Substitui "e-mail"
     * @param email novo "e-mail"
     */
    public void setEmail(String email) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.email = email;
    }
    
    /**
     * Substitui "grupo de investigação"
     * @param grupo novo "grupo de investigação"
     */
    public void setGrupo(Grupo grupo) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.grupo = grupo;
    }

    /**
     * Substitui "lista de publicações"
     * @param publicacoes nova "lista de publicações"
     */
    public void setPublicacoes(ArrayList<Publicacao> publicacoes) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.publicacoes = publicacoes;
    }
    
    /**
     * Determina o tipo de investigador
     * @return tipo de investigador em String
     */
    public abstract String tipo(); //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    
    /**
     * Determina como é escrito o nome do investigador na lista de autores de uma publicação
     * @return nome do investigador na lista de autores de uma publicação em String
     */
    public abstract String nomeAutor(); //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome + "\n->E-mail: " + email + "\n->Grupo: " + grupo.toString();
    }
}
