
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Estudante extends Investigador { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private String tituloTese;
    private Data dataConclusao;
    private Efetivo orientador;
    
    /**
     * Construtor de objetos do tipo "Estudante" com parâmetros string
     * @param nome nome do estudante
     * @param email e-mail do estudante
     * @param tituloTese título da tese de mestrado do estudante
     * @param data data de conclusão do mestrado do estudante
     */
    public Estudante(String nome,String email,String tituloTese,String data) { //--*--*--*--*--*--*--*--*--*--*
        super(nome,email);
        this.tituloTese = tituloTese;
        dataConclusao = new Data(data,"futuro");
    }

    /**
     * Devolve "título da tese de mestrado do estudante"
     * @return tituloTese
     */
    public String getTituloTese() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return tituloTese;
    }

    /**
     * Devolve "data de conclusão da tese de mestrado do estudante"
     * @return dataConclusao
     */
    public Data getDataConclusao() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return dataConclusao;
    }

    /**
     * Devolve "investigador efetivo orientador do estudante"
     * @return orientador
     */
    public Efetivo getOrientador() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return orientador;
    }

    /**
     * Substitui "título da tese de mestrado do estudante"
     * @param tituloTese novo "título da tese de mestrado do estudante" 
     */
    public void setTituloTese(String tituloTese) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.tituloTese = tituloTese;
    }

    /**
     * Substitui "data de conclusão da tese de mestrado do estudante"
     * @param dataConclusao nova "data de conclusão da tese de mestrado do estudante"
     */
    public void setDataConclusao(Data dataConclusao) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.dataConclusao = dataConclusao;
    }
    
    /**
     * Substitui "investigador efetivo orientador do estudante"
     * @param orientador novo "investigador efetivo orientador do estudante"
     */
    public void setOrientador(Efetivo orientador) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.orientador = orientador;
    }

    /**
     * Determina o tipo de investigador
     * @return tipo de investigador em String
     */
    @Override
    public String tipo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return "Estudante";
    }
    
    /**
     * Determina como é escrito o nome do investigador na lista de autores de uma publicação
     * @return nome do investigador na lista de autores de uma publicação em String
     */
    @Override
    public String nomeAutor() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome.charAt(0) + ". " + nome.split(" ")[nome.split(" ").length - 1];
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return  super.toString() + "\n->Orientador: " + orientador.nomeAutor()
                + "\n->Título Tese: " + tituloTese  + "\n->Data conclusão: " + dataConclusao.toString(); 
    }
}
