
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Conferencia extends Artigo { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private String localizacao;
    
    /**
     * Construtor de objetos do tipo "Artigo" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     * @param nome nome do artigo
     * @param data data do artigo
     * @param localizacao localizaoção do artigo de conferência
     */
    public Conferencia(String titulo,String palavraChave,int ano, int audiencia,String resumo,String nome,String data,String localizacao) { //--*
        super(titulo,palavraChave,ano,audiencia,resumo,nome,data);
        this.localizacao = localizacao;
    }

    /**
     * Devolve "localização do artigo de conferência"
     * @return localizacao
     */
    public String getLocalizacao() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return localizacao;
    } 

    /**
     * Substitui "localização do artigo de conferência"
     * @param localizacao nova "localização do artigo de conferência"
     */
    public void setLocalizacao(String localizacao) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.localizacao = localizacao;
    }
    
   /**
     * Determina o tipo de publicação
     * @return tipo de publicação em String
     */
    @Override
    public String tipo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    return "Artigo de Conferência"; 
    }
    
    /**
     * Determina o fator de impacto da publicação
     * @return fator de impacto da publicação em String
     */
    @Override
    public String calculaFator() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        if(audiencia>=500) return "A"; 
        else if(audiencia>=200 && audiencia<500) return "B"; 
        else return "C"; 
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return super.toString() + "\n->Localização: " + localizacao;
    }
}
