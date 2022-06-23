package projeto;

import java.io.Serializable;
import java.time.Year;
import java.util.ArrayList;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Grupo implements Serializable { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
    private String nome;
    private String acronimo;
    private Investigador responsavel;
    private ArrayList<Investigador> investigadores;

    /**
     * Construtor de objetos do tipo "Grupo" com parâmetros string
     * @param nome nome do grupo
     * @param acronimo acrónimo do grupo
     */
    public Grupo(String nome, String acronimo) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
        this.acronimo = acronimo;
        investigadores = new ArrayList<>();
    }
    
    /**
     * Devolve "nome"
     * @return nome
     */
    public String getNome() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome;
    }

    /**
     * Devolve "acronimo"
     * @return acronimo
     */
    public String getAcronimo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return acronimo;
    }
    
    /**
     * Devolve "investigador responsavel"
     * @return responsavel
     */
    public Investigador getResponsavel() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return responsavel;
    }

    /**
     * Devolve "lista de investigadores"
     * @return investigadores
     */
    public ArrayList<Investigador> getInvestigadores() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return investigadores;
    }

    /**
     * Substitui "nome"
     * @param nome novo "nome"
     */
    public void setNome(String nome) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.nome = nome;
    }

    /**
     * Substitui "acronimo"
     * @param acronimo novo "acronimo"
     */
    public void setAcronimo(String acronimo) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.acronimo = acronimo;
    }
    
    /**
     * Substitui "investigador responsavel"
     * @param responsavel novo "investigador responsavel"
     */
    public void setResponsavel(Investigador responsavel) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.responsavel = responsavel;
    }

    /**
     * Substitui "lista de investigadores"
     * @param investigadores nova "lista de investigadores"
     */
    public void setInvestigadores(ArrayList<Investigador> investigadores) { //--*--*--*--*--*--*--*--*--*
        this.investigadores = investigadores;
    }

    /**
     * Determina a lista de investigadores Efetivos no grupo
     * @return lista de investigadores Efetivos no grupo
     */
    public ArrayList<Efetivo> getEfetivos() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        ArrayList<Efetivo> efetivos = new ArrayList<>();
        for (int i = 0; i < investigadores.size(); i++) {
            if (investigadores.get(i).tipo().equals("Efetivo")) {
                efetivos.add((Efetivo)investigadores.get(i));
            }
        }
        return efetivos;
    }
    
    /**
     * Determina a lista de investigadores Estudantes no grupo
     * @return lista de investigadores Estudantes no grupo 
     */
    public ArrayList<Estudante> getEstudantes() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        ArrayList<Estudante> estudantes = new ArrayList<>();int n = 0;
        for (int i = 0; i < investigadores.size(); i++) {
            if (investigadores.get(i).tipo().equals("Estudante")) {
                estudantes.add((Estudante)investigadores.get(i));
            }
        }
        return estudantes;
    }

    /**
     * Determina a lista de publicações do grupo, sem repetições
     * @return lista de publicações do grupo, sem repetições
     */
    public ArrayList<Publicacao> publicacoesGrupo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        ArrayList<Publicacao> lista = new ArrayList<>();
        for (int i = 0; i < (investigadores.size()); i++) {
            Investigador investigador = investigadores.get(i);
            for (int j = 0; j < (investigador.getPublicacoes().size()); j++) {
                Publicacao publicacao = investigador.getPublicacoes().get(j);
                //evitar repetição de publicações
                if (!lista.contains(publicacao)) {
                    lista.add(publicacao);
                }
            }
        }
        return lista;
    }

    /**
     * Determina a lista de publicações do grupo dos últimos 5 anos, sem repetições
     * @return lista de publicações do grupo dos últimos 5 anos, sem repetições
     */
    public ArrayList<Publicacao> publicacoesGrupo5Anos() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        ArrayList<Publicacao> lista = new ArrayList<>();
        for (int i = 0; i < (investigadores.size()); i++) {
            Investigador investigador = investigadores.get(i);
            for (int j = 0; j < (investigador.getPublicacoes().size()); j++) {
                Publicacao publicacao = investigador.getPublicacoes().get(j);
                //evitar repetição de publicações
                if (!lista.contains(publicacao) && publicacao.getAno() > Year.now().getValue()-5) {
                    lista.add(publicacao);
                }
            }
        }
        return lista;
    }

    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return nome + " (" + acronimo + ")";
    }
}
