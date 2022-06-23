package projeto;

import java.util.Comparator;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class ComparaFator implements Comparator<Publicacao> { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    /**
     * 
     * @param p1
     * @param p2
     * @return 
     */
    @Override
    public int compare(Publicacao p1, Publicacao p2) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return p1.calculaFator().compareTo(p2.calculaFator());
    }
}
