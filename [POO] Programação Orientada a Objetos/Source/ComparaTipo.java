package projeto;

import java.util.Comparator;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class ComparaTipo implements Comparator<Publicacao> { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    /**
     * 
     * @param p1
     * @param p2
     * @return 
     */
    @Override
    public int compare(Publicacao p1, Publicacao p2) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return p1.tipo().compareTo(p2.tipo());
    }
}
