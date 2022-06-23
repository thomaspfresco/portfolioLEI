
package projeto;

import java.util.Comparator;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class ComparaPublicacoes implements Comparator<Publicacao> { //--*--*--*--*--*--*--*--*--*--*--*--*--*
    
    /**
     * 
     * @param p1
     * @param p2
     * @return 
     */
    @Override
    public int compare(Publicacao p1, Publicacao p2) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        if (p1.getAno() == p2.getAno()) {
            
            if (p1.tipo().equals(p2.tipo())) {
                
                if (p1.calculaFator().equals(p2.calculaFator())) {
                    
                    return 0;
                
                } else return p1.calculaFator().compareTo(p2.calculaFator());

            } else return p1.tipo().compareTo(p2.tipo());
            
         } else return p1.getAno() - p2.getAno();
    }
}

