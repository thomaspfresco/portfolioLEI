
package projeto;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Efetivo extends Investigador {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private int nGabinete, nTelefone;
    
    /**
     * Construtor de objetos do tipo "Efetivo" com parâmetros string e int
     * @param nome nome do efetivo
     * @param email e-mail do efetivo
     * @param nGabinete numero de gabinete do efetivo
     * @param nTelefone numero de telefone do efetivo
     */
    public Efetivo(String nome,String email,int nGabinete,int nTelefone) {  //--*--*--*--*--*--*--*--*--*--*--*
        super(nome,email);
        this.nGabinete = nGabinete;
        this.nTelefone = nTelefone;
    }

    /**
     * Devolve "numero de gabinete do efetivo"
     * @return nGabinete
     */
    public int getnGabinete() {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nGabinete;
    }

    /**
     * Devolve "numero de telefone do efetivo"
     * @return nTelefone
     */
    public int getnTelefone() {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nTelefone;
    }

    /**
     * Substitui "numero de gabinete do efetivo"
     * @param nGabinete novo "numero de gabinete do efetivo"
     */
    public void setnGabinete(int nGabinete) {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nGabinete = nGabinete;
    }

    /**
     * Substitui "numero de telefone do efetivo"
     * @param nTelefone novo "numero de telefone do efetivo"
     */
    public void setnTelefone(int nTelefone) {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nTelefone = nTelefone;
    }

    /**
     * Determina o tipo de investigador
     * @return tipo de investigador em String
     */
    @Override
    public String tipo() {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return "Efetivo";
    }
    
    /**
     * Determina como é escrito o nome do investigador na lista de autores de uma publicação
     * @return nome do investigador na lista de autores de uma publicação em String
     */
    @Override
    public String nomeAutor() {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return "Professor(a) " + nome;
    }
    
    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() {  //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return  super.toString() + "\n->Nº Gabinete: " + nGabinete
                + "\n->Nº Telefone: " + nTelefone; 
    }
}
