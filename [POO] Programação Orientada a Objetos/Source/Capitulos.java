
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Capitulos extends Livro { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private String nome;
    private int pagInicio, pagFim;
    
    /**
     * Construtor de objetos do tipo "Capitulos" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     * @param editora editora do livro
     * @param isbn isbn do livro
     * @param nome nome do capítulo
     * @param pagInicio página de início do capítulo
     * @param pagFim página do fim do capítulo
     */
    public Capitulos(String titulo,String palavraChave,int ano, int audiencia,String resumo,String editora,int isbn,String nome,int pagInicio,int pagFim) { //--*
        super(titulo,palavraChave,ano,audiencia,resumo,editora,isbn);
        this.nome = nome;
        this.pagInicio = pagInicio;
        this.pagFim = pagFim;
    }

    /**
     * Devolve "nome do capítulo"
     * @return nome
     */
    public String getNome() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome;
    }

    /**
     * Devolve "página de início do capítulo"
     * @return pagInicio
     */
    public int getPagInicio() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return pagInicio;
    }

    /**
     * Devolve "página do fim do capítulo"
     * @return pagFim
     */
    public int getPagFim() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return pagFim;
    }

    /**
     * Substitui "nome do capítulo"
     * @param nome novo "nome do capítilo"
     */
    public void setNome(String nome) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
    }

    /**
     * Substitui "página de início do capítulo"
     * @param pagInicio nova "página de início do capítulo"
     */
    public void setPagInicio(int pagInicio) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.pagInicio = pagInicio;
    }

    /**
     * Substitui "página do fim do capítulo"
     * @param pagFim nova "página do fim do capítulo"
     */
    public void setPagFim(int pagFim) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.pagFim = pagFim;
    }
    
    /**
     * Determina o tipo de publicação
     * @return tipo de publicação em String
     */
    @Override
    public String tipo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    return "Livro de Capítulos"; 
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return super.toString() + "\n->Nome Capítulo: " + nome + "\n->Pág. Início: " + pagInicio + "  ->Pág. Fim: " + pagFim;
    }
}
