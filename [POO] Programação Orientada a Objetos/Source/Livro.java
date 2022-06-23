
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Livro extends Publicacao { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    protected String editora;
    protected int isbn; 
    
    /**
     * Construtor de objetos do tipo "Livro" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     * @param editora editora do livro
     * @param isbn isbn do livro
     */
    public Livro(String titulo,String palavraChave,int ano, int audiencia,String resumo,String editora,int isbn) { //--*
        super(titulo,palavraChave,ano,audiencia,resumo);
        this.editora = editora;
        this.isbn = isbn;
    }

    /**
     * Devolve "editora do livro"
     * @return editora
     */
    public String getEditora() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return editora;
    }

    /**
     * Devolve "isbn do livro"
     * @return isbn
     */
    public int getIsbn() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return isbn;
    }

    /**
     * Substitui "editora do livro"
     * @param editora nova "editora do livro"
     */
    public void setEditora(String editora) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.editora = editora;
    }

    /**
     * Substitui "isbn do livro"
     * @param isbn novo "isbn do livro"
     */
    public void setIsbn(int isbn) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.isbn = isbn;
    }
    
    /**
     * Determina o tipo de publicação
     * @return tipo de publicação em String
     */
    @Override
    public String tipo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    return "Livro"; 
    }
    
    /**
     * Determina o fator de impacto da publicação
     * @return fator de impacto da publicação em String
     */
    @Override
    public String calculaFator() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        if(audiencia>=10000) return "A"; 
        else if(audiencia>=5000 && audiencia<10000) return "B"; 
        else return "C"; 
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return super.toString() + "\n->Editora: " + editora + "  ->ISBN: " + isbn;
    }
}
