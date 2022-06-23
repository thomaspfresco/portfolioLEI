
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */  
public abstract class Artigo extends Publicacao {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    protected String nome;
    protected Data data;
    
    /**
     * Construtor de objetos do tipo "Artigo" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     * @param nome nome do artigo
     * @param data data do artigo
     */
    public Artigo(String titulo,String palavraChave,int ano, int audiencia,String resumo,String nome,String data) {  //--*
        super(titulo,palavraChave,ano,audiencia,resumo);
        this.nome = nome;
        this.data = new Data(data,"passado");
    }

    /**
     * Devolve "nome do artigo"
     * @return nome
     */
    public String getNome() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome;
    }

    /**
     * Devolve "data do artigo"
     * @return data
     */
    public Data getData() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return data;
    }

    /**
     * Substitui "nome do aritgo"
     * @param nome novo "nome do artigo"
     */
    public void setNome(String nome) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
    }
    
    /**
     * Substitui "data do artigo"
     * @param data nova "data do artigo"
     */
    public void setData(Data data) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.data = data;
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return super.toString() + "\n->Nome: " + nome + "\n->Data: " + data.toString();
    }
}
