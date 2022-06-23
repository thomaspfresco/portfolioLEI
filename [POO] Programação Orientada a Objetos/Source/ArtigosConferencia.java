
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class ArtigosConferencia extends Livro { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private String nome;
    private int nArtigos;
    
    /**
     * Construtor de objetos do tipo "Capitulos" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     * @param editora editora do livro
     * @param isbn isbn do livro
     * @param nome nome do artigo de conferência
     * @param nArtigos numero de artigos de conferência
     */
    public ArtigosConferencia(String titulo,String palavraChave,int ano, int audiencia,String resumo,String editora,int isbn,String nome,int nArtigos) { //--*
        super(titulo,palavraChave,ano,audiencia,resumo,editora,isbn);
        this.nome = nome;
        this.nArtigos = nArtigos;
    }

    /**
     * Devolve "nome do artigo de conferência"
     * @return nome
     */
    public String getNome() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome;
    }

    /**
     * Devolve "numero de artigos de conferência"
     * @return nArtigos
     */
    public int getnArtigos() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nArtigos;
    }

    /**
     * Substitui "nome do artigo de conferência"
     * @param nome novo "nome do artigo de conferência" 
     */
    public void setNome(String nome) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
    }

    /**
     * Substitui "numero de artigos de conferência"
     * @param nArtigos novo "numero de artigos de conferência"
     */
    public void setnArtigos(int nArtigos) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nArtigos = nArtigos;
    }
    
    /**
     * Determina o tipo de publicação
     * @return tipo de publicação em String
     */
    @Override
    public String tipo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    return "Livro de Artigos de conferência"; 
    }
    
    /**
     * Determina o fator de impacto da publicação
     * @return fator de impacto da publicação em String
     */
    @Override
    public String calculaFator() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        if(audiencia>=7500) return "A"; 
        else if(audiencia>=2500 && audiencia<7500) return "B"; 
        else return "C"; 
    }

    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return super.toString() + "\n->Nome Conferência: " + nome + "\n->Nº Artigos: " + nArtigos;
    }
}