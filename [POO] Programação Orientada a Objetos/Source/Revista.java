
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Revista extends Artigo { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private int numero;
    
    /**
     * Construtor de objetos do tipo "Revista" com parâmetros string e int
     * @param titulo titulo da publicação
     * @param palavraChave palavra-chave da publicação
     * @param ano ano de publicação
     * @param audiencia audiencia da publicação
     * @param resumo resumo da publicação
     * @param nome nome do artigo
     * @param data data do artigo
     * @param numero número de revista
     */
    public Revista(String titulo,String palavraChave,int ano, int audiencia,String resumo,String nome,String data,int numero) { //--*
        super(titulo,palavraChave,ano,audiencia,resumo,nome,data);
        this.numero = numero;
    }

    /**
     * Devolve "número de revista"
     * @return  numero
     */
    public int getNumero() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return numero;
    }

    /**
     * Substitui "número de revista"
     * @param numero novo "número de revista"
     */
    public void setNumero(int numero) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.numero = numero;
    }
    
    /**
     * Determina o tipo de publicação
     * @return tipo de publicação em String
     */
    @Override
    public String tipo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    return "Artigo de Revista"; 
    }
    
    /**
     * Determina o fator de impacto da publicação
     * @return fator de impacto da publicação em String
     */
    @Override
    public String calculaFator() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        if(audiencia>=1000) return "A"; 
        else if(audiencia>=500 && audiencia<1000) return "B"; 
        else return "C"; 
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return super.toString() + "\n->Número: " + numero;
    }
}

