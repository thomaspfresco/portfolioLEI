package projeto;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.time.Year;
import java.util.ArrayList;
import java.util.Collections;
import java.util.InputMismatchException;
import java.util.Scanner;
import java.util.Random;

/**
 *
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Cisuc {

    private ArrayList<Grupo> grupos;

    /**
     * Construtor de objetos do tipo "Cisuc"
     */
    public Cisuc() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        grupos = new ArrayList<>();
    }

    /**
     * Método "main" Cria um objeto do tipo "Cisuc" Chama os métodos
     * "recolherInfo()", "menu()", "guardar()"
     *
     * @param args
     */
    public static void main(String[] args) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        Cisuc cisuc = new Cisuc();
        cisuc.recolherInfo();
        cisuc.menu();
        cisuc.guardar();
    }

    /**
     * Devolve "grupos"
     *
     * @return grupos
     */
    public ArrayList<Grupo> getGrupos() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return grupos;
    }

    /**
     * Substitui "grupos"
     *
     * @param grupos novo "grupos"
     */
    public void setGrupos(ArrayList<Grupo> grupos) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.grupos = grupos;
    }

    /**
     * Imprime o menu e, consoante a opção inserida pelo utlizador, chama
     * determinados métodos Se inserido "0", termina o método
     */
    private void menu() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        int opcao = -1;
        Scanner sc;
        
        do { //Menu
            try {
                //print menu
                System.out.println("\n\n*------------------C-I-S-U-S------------------*");
                System.out.println("| 1 - Indicadores gerais do CISUC             |");
                System.out.println("| 2 - Publicações de um grupo de investigação |");
                System.out.println("| 3 - Membros de um grupo de investigação     |");
                System.out.println("| 4 - Publicações de um investigador          |");
                System.out.println("| 5 - Grupos de investigação                  |");
                System.out.println("| 0 - Sair                                    |");
                System.out.println("*---------------------------------------------*\n");
                System.out.print("OPÇÃO: ");
                sc = new Scanner(System.in);
                opcao = sc.nextInt();

                if (grupos.isEmpty()) {
                    System.out.println("Cisus vazio!"); //cisus vazio
                } else {
                    switch (opcao) {
                        case 1 ->
                            indicadoresGerais();
                        case 2 ->
                            publicacoesGrupo();
                        case 3 ->
                            membrosGrupo();
                        case 4 ->
                            publicacoesInvestigador();
                        case 5 ->
                            gruposInvestigacao();
                        case 0 -> {
                        }
                        default ->
                            //numero inteiro não correspondente a uma opção
                            System.out.println("Input inválido.");
                    }
                }
                //input não inteiro
            } catch (InputMismatchException ex) {
                System.out.println("Input inválido.");
                opcao = -1;
            }
        } while (opcao != 0);
    }

    /**
     * permite selecionar um grupo presente no Cisuc
     *
     * @param titulo representa o cabeçalho a imprimir
     * @return grupo encontrado, ou "NULL", caso não seja encontrado
     */
    private Grupo selecaoGrupo(String titulo) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        String opcao = "";
        Grupo grupo;
        Scanner sc;

        System.out.println(titulo);
        System.out.println("(Opções possíveis: AC ; CMS ; ECOS ; IS ; LCT ; SSE)");
        System.out.print("GRUPO: ");

        sc = new Scanner(System.in);
        opcao = sc.nextLine();

        for (int i = 0; i < (grupos.size()); i++) {
            grupo = grupos.get(i);
            if (opcao.toUpperCase().equals(grupo.getAcronimo())) {
                return grupo;
            }
        }
        return null;
    }

    /**
     * permite selecionar um investigador presente no Cisuc (pelo nome ou
     * e-mail)
     *
     * @return investigador encontrado, ou "NULL", caso não seja encontrado
     */
    private Investigador selecaoInvestigador() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        String opcao = "";
        Investigador investigador;
        Scanner sc;
        System.out.print("Investigador(nome ou e-mail): ");

        sc = new Scanner(System.in);
        opcao = sc.nextLine();
        String aux = opcao.toUpperCase();

        for (int i = 0; i < listaInvestigadores().size(); i++) {
            investigador = listaInvestigadores().get(i);
            if (aux.equals(investigador.getNome().toUpperCase()) || opcao.equals(investigador.getEmail())) {
                return investigador;
            }
        }
        return null;
    }

    /**
     * determina a lista de todos os invstigadores do Cisuc, sem repetições
     *
     * @return lista de todos os invstigadores do Cisuc, sem repetições
     */
    public ArrayList<Investigador> listaInvestigadores() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        ArrayList<Investigador> lista = new ArrayList<>();
        for (int i = 0; i < (grupos.size()); i++) {
            Grupo grupo = grupos.get(i);
            for (int r = 0; r < (grupo.getInvestigadores().size()); r++) {
                Investigador investigador = grupo.getInvestigadores().get(r);
                if (!lista.contains(investigador)) {
                    lista.add(investigador);
                }
            }
        }
        return lista;
    }

    /**
     * determina a lista de todas as publicações do Cisuc, sem repetições
     *
     * @return lista de todas as publicações do Cisuc, sem repetições
     */
    public ArrayList<Publicacao> listaPublicacoes() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        ArrayList<Publicacao> lista = new ArrayList<>();
        for (int i = 0; i < (grupos.size()); i++) {
            Grupo grupo = grupos.get(i);
            for (int r = 0; r < (grupo.getInvestigadores().size()); r++) {
                Investigador investigador = grupo.getInvestigadores().get(r);
                for (int s = 0; s < (investigador.getPublicacoes().size()); s++) {
                    Publicacao publicacao = investigador.getPublicacoes().get(s);
                    //evitar repetição de publicações
                    if (!lista.contains(publicacao)) {
                        lista.add(publicacao);
                    }
                }
            }
        }
        return lista;
    }

    /**
     * Apresentar os indicadores gerais do CISUC (ponto 1)
     */
    private void indicadoresGerais() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

        System.out.println("\nINDICADORES GERAIS ----------//----------//----------");
        int nEstudantes = 0, nEfetivos = 0, nPublicacoes = 0;

        //contar o número de membros estudante e efetivos
        for (int i = 0; i < (listaInvestigadores().size()); i++) {
            Investigador investigador = listaInvestigadores().get(i);
            if (investigador.tipo().equals("Estudante")) {
                nEstudantes += 1;
            } else if (investigador.tipo().equals("Efetivo")) {
                nEfetivos += 1;
            }
        }

        //contagem do numero de publicações dos últimos 5 anos
        for (int i = 0; i < (listaPublicacoes().size()); i++) {
            Publicacao publicacao = listaPublicacoes().get(i);
            if (publicacao.getAno() > Year.now().getValue() - 5) {
                nPublicacoes += 1;
            }
        }

        //imprimir os dados recolhidos das contagens
        System.out.println("-> Nº Membros Cisus: " + listaInvestigadores().size());
        System.out.println("-> Nº Estudantes: " + nEstudantes);
        System.out.println("-> Nº Efetivos: " + nEfetivos);
        System.out.println("-> Nº publicações últimos 5 anos: " + nPublicacoes);
        //imprimir o nº de publicações de cada tipo 
        agrupaPorTipo(listaPublicacoes(),"-> ");
    }

    /**
     * Listar as publicações de um grupo de investigação, dos últimos 5 anos,
     * organizadas por ano, por tipo de publicação e por fator de impacto (ponto
     * 2)
     */
    private void publicacoesGrupo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

        Grupo grupo = selecaoGrupo("\nPUBLICAÇÕES GRUPO ----------//----------//----------");
        if (grupo == null) {
            System.out.println("\nGrupo não existe na Cisuc.");
        } else {
            ArrayList<Publicacao> p = grupo.publicacoesGrupo5Anos();
            System.out.println();
            organizar(p);
        }
    }

    /**
     * Listar os membros de um grupo de investigação agrupados por categoria
     * (ponto 3)
     */
    private void membrosGrupo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

        Grupo grupo = selecaoGrupo("\nMEMBROS GRUPO ----------//----------//----------");
        if (grupo == null) {
            System.out.println("Grupo inserido não existe na Cisuc.");
        } else {
            Investigador investigador;
            System.out.println(); //estético

            //seleção efetivos
            System.out.println("Efetivos: ------------------------------");
            for (int i = 0; i < grupo.getEfetivos().size(); i++) {
                investigador = grupo.getEfetivos().get(i);
                System.out.println(investigador.toString());
                System.out.println(); //estético
            }

            //escrita estudantes
            System.out.println("Estudantes: ------------------------------");
            for (int i = 0; i < grupo.getEstudantes().size(); i++) {
                investigador = grupo.getEstudantes().get(i);
                System.out.println(investigador.toString());
                System.out.println(); //estético
            }
        }
    }

    /**
     * Listar as publicações de um investigador agrupadas por ano, tipo de
     * publicação e fator de impacto (ponto 4)
     */
    private void publicacoesInvestigador() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

        System.out.println("\nPUBLICAÇÕES INVESTIGADOR ----------//----------//----------");
        Investigador investigador = selecaoInvestigador();
        if (investigador == null) {
            System.out.println("\nInvestigador inserido não existe na Cisuc.");
        } else {
            organizar(investigador.getPublicacoes());
        }
    }

    /**
     * Listar todos os grupos de investigação, e apresentar para cada grupo
     * determinados dados (ponto 5)
     */
    private void gruposInvestigacao() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

        System.out.println("\nGRUPOS INVESTIGAÇÃO ----------//----------//----------");
        String opcao;
        Grupo grupo;
        Scanner sc;

        ArrayList<Publicacao> p;

        for (int i = 0; i < grupos.size(); i++) {
            grupo = grupos.get(i);
            p = grupo.publicacoesGrupo5Anos();
            System.out.println(grupo.toString() + " ----------//----------//----------\n");
            System.out.println("-> Nº Membros: " + grupo.getInvestigadores().size());
            System.out.println("-> Nº Efetivos: " + grupo.getEfetivos().size());
            System.out.println("-> Nº Estudantes: " + grupo.getEstudantes().size());
            System.out.println("-> Nº publicações últimos 5 anos: " + p.size());
            //números por ano
            System.out.println("-> Nº publicações últimos 5 anos (por ano) --//--//--");
            agrupaPorAno(p,"\t-> ");
            //números por tipo
            System.out.println("-> Nº publicações últimos 5 anos (por tipo) --//--//--");
            agrupaPorTipo(p,"\t-> ");
            //números por fator de impacto
            System.out.println("-> Nº publicações últimos 5 anos (por fator de impacto) --//--//--");
            agrupaPorImpacto(p,"\t-> ");
            System.out.println("\n"); //estético
        }
    }

    /**
     * Organiza e imprime uma arraylist de publicações
     *
     * @param p arraylist de publicações a organizar
     */
    public void organizar(ArrayList<Publicacao> p) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        Collections.sort(p, new ComparaPublicacoes());
        Publicacao publicacao;

        for (int i = 0; i < p.size(); i++) {
            publicacao = p.get(i);
            //primeira publicação da lista
            if (i == 0) {
                System.out.println("-> Publicações do ano " + publicacao.getAno() + ": --//--//--");
                System.out.println("\t-> Publicações do tipo " + publicacao.tipo() + ": --//--//--");
                System.out.println("\t\t-> Publicações de fator de impacto " + publicacao.calculaFator() + ": --//--//--\n");
                publicacao.infoPrincipal("\t\t\t");
            } else {
                //ano da publicação i é igual ao da publicação i-1
                if (publicacao.getAno() == p.get(i - 1).getAno()) {
                    //tipo da publicação i é igual ao da publicação i-1
                    if (publicacao.tipo().equals(p.get(i - 1).tipo())) {
                        //fator da publicação i é igual ao da publicação i-1
                        if (publicacao.calculaFator().equals(p.get(i - 1).calculaFator())) {
                            publicacao.infoPrincipal("\t\t\t");
                            //fator da publicação i é diferente da publicação i-1
                        } else {
                            System.out.println("\t\t-> Publicações de fator de impacto " + publicacao.calculaFator() + ": --//--//--\n");
                            publicacao.infoPrincipal("\t\t\t");
                        }
                        //tipo da publicação i é diferente da publicação i-1
                    } else {
                        System.out.println("\t-> Publicações do tipo " + publicacao.tipo() + ": --//--//--");
                        System.out.println("\t\t-> Publicações de fator de impacto " + publicacao.calculaFator() + ": --//--//--\n");
                        publicacao.infoPrincipal("\t\t\t");
                    }
                    //ano da publicação i é diferente da publicação i-1
                } else {
                    System.out.println("-> Publicações do ano " + publicacao.getAno() + ": --//--//--");
                    System.out.println("\t-> Publicações do tipo " + publicacao.tipo() + ": --//--//--");
                    System.out.println("\t\t-> Publicações de fator de impacto " + publicacao.calculaFator() + ": --//--//--\n");
                    publicacao.infoPrincipal("\t\t\t");
                }
            }
        }
    }

    /**
     * Agrupa uma lista de publicações por ano e escreve os respetivos números
     *
     * @param p lista que vais sofrer o agrupamento
     * @param esp define como é iniciada a escrita dos dados (estético)
     */
    public void agrupaPorAno(ArrayList<Publicacao> p, String esp) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        //organizar
        Collections.sort(p, new ComparaPublicacoes());

        ArrayList<Publicacao> aux = new ArrayList<>();
        Publicacao publicacao;

        int ano = p.get(0).getAno();
        for (int i = 0; i < p.size(); i++) {
            publicacao = p.get(i);

            if (publicacao.getAno() == ano) {
                aux.add(publicacao);
            } else {
                System.out.println(esp + "Publicações de " + ano + ": " + aux.size());
                ano = publicacao.getAno();
                aux.clear();
                aux.add(publicacao);
            }
        }
        //imprime último array auxiliar (publicações de um último ano)
        System.out.println(esp + "Publicações de " + ano + ": " + aux.size());
    }

    /**
     * Agrupa uma lista de publicações por tipo e escreve os respetivos números
     *
     * @param p lista que vais sofrer o agrupamento
     * @param esp define como é iniciada a escrita dos dados (estético)
     */
    public void agrupaPorTipo(ArrayList<Publicacao> p, String esp) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        //organizar
        Collections.sort(p, new ComparaTipo());

        ArrayList<Publicacao> aux = new ArrayList<>();
        Publicacao publicacao;

        String tipo = p.get(0).tipo();
        for (int i = 0; i < p.size(); i++) {
            publicacao = p.get(i);

            if (publicacao.tipo().equals(tipo)) {
                aux.add(publicacao);
            } else {
                System.out.println(esp + "Publicações do tipo " + tipo + ": " + aux.size());
                tipo = publicacao.tipo();
                aux.clear();
                aux.add(publicacao);
            }
        }
        //imprime último array auxiliar (publicações de um último tipo)
        System.out.println(esp + "Publicações do tipo " + tipo + ": " + aux.size());
    }

    /**
     * Agrupa uma lista de publicações por fator de impacto e escreve os respetivos números
     *
     * @param p lista que vais sofrer o agrupamento
     * @param esp define como é iniciada a escrita dos dados (estético)
     */
    public void agrupaPorImpacto(ArrayList<Publicacao> p, String esp) { //--*--*--*--*--*--*--*--*--*--*--*--*--*
        //organizar
        Collections.sort(p, new ComparaFator());

        ArrayList<Publicacao> aux = new ArrayList<>();
        Publicacao publicacao;

        String fator = p.get(0).calculaFator();
        for (int i = 0; i < p.size(); i++) {
            publicacao = p.get(i);

            if (publicacao.calculaFator().equals(fator)) {
                aux.add(publicacao);
            } else {
                System.out.println(esp + "Publicações de fator " + fator + ": " + aux.size());
                fator = publicacao.calculaFator();
                aux.clear();
                aux.add(publicacao);
            }
        }
        //imprime último array auxiliar (publicações de um último ofator)
        System.out.println(esp + "Publicações de fator " + fator + ": " + aux.size());
    }

    /**
     * Tenta ler informação de um ficheiro de objetos "Cisuc.obj" Se não
     * existir, chama os métodos "recolherGrupos()", "recolherInvestigadores()"
     * e "recolherPublicacoes()"
     */
    private void recolherInfo() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        File fobj = new File("Cisuc.obj");

        //if (fobj.exists() && fobj.isFile()) 
        try {
            FileInputStream fis = new FileInputStream(fobj);
            ObjectInputStream ois = new ObjectInputStream(fis);

            //inserir os alunos
            grupos = (ArrayList<Grupo>) ois.readObject();
            System.out.println("Cisuc atualizado com SUCESSO.");
            ois.close();

            //ficheiro .obj não existe 
        } catch (FileNotFoundException ex) {
            System.out.println("Ficheiro 'Cisuc.obj' não disponível.");
            recolherGrupos();
            recolherInvestigadores();
            recolherPublicacoes();
        } catch (IOException ex) {
            System.out.println("Erro a ler ficheiro 'Cisuc.obj'!");
        } catch (ClassNotFoundException ex) {
            System.out.println("Erro a converter objeto!");
        }
    }

    /**
     * recolhe informação de um ficheiro ".txt" onde estão descritos os grupos
     */
    private void recolherGrupos() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        File f = new File("Grupos.txt");

        //if (fobj.exists() && fobj.isFile()) 
        try {
            FileReader fr = new FileReader(f);
            BufferedReader br = new BufferedReader(fr);
            String line;
            line = br.readLine(); //saltar linha do título

            //inserir os Grupos
            Grupo grupo;
            while ((line = br.readLine()) != null) {
                String[] info = line.split("#");
                grupo = new Grupo(info[0], info[1]);
                //verificar se existe um grupo com o mesmo nome ou acrónimo
                if (checkGrupos(grupo) == false) {
                    grupos.add(grupo);
                } else {
                    System.out.println("Já existe um grupo com o nome " + grupo.getNome() + " ou com o acrónimo " + grupo.getAcronimo() + " no Cisuc.");
                }
            }
            System.out.println("Grupos de investigação adicionados com SUCESSO.");
            br.close();

        } catch (FileNotFoundException ex) {
            System.out.println("Erro a abrir o ficheiro 'Grupos.txt'!");
        } catch (IOException ex) {
            System.out.println("Erro a ler o ficheiro 'Grupos.txt'!");
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.out.println("O ficheiro 'Grupos.txt' não cumpre a estrutura adequada!");
        }

    }

    /**
     * recolhe informação de um ficheiro ".txt" onde estão descritos os
     * investigadores
     */
    private void recolherInvestigadores() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        File f = new File("Investigadores.txt");

        //if (fobj.exists() && fobj.isFile()) 
        try {
            FileReader fr = new FileReader(f);
            BufferedReader br = new BufferedReader(fr);
            String line;
            line = br.readLine(); //saltar linha do título
            Grupo grupo;

            //inserir os Grupos
            Investigador investigador;
            while ((line = br.readLine()) != null) {
                String[] info = line.split("#");
                //investigadores efetivos
                if (checkNumeric(info[3])) {
                    investigador = new Efetivo(info[0], info[1], Integer.parseInt(info[3]), Integer.parseInt(info[4]));
                } //investigadores estudantes
                else {
                    investigador = new Estudante(info[0], info[1], info[3], info[4]);
                }

                //adicionar ao respetivo grupo
                boolean verif = false;
                if (grupos.isEmpty()) {
                    System.out.println("Cisus vazio!");
                } else {
                    //procura do grupo correspondente
                    for (int i = 0; i < grupos.size(); i++) {
                        grupo = grupos.get(i);

                        //grupo correspondente é encontrado
                        if (grupo.getAcronimo().equals(info[2])) {

                            //verificar se já existe um investigador com o mesmo email (único) no Cisuc
                            if (checkInvestigadores(investigador, listaInvestigadores()) == false) {
                                grupo.getInvestigadores().add(investigador);
                                investigador.setGrupo(grupo);

                                //designar como responsável caso o seja
                                if (info.length == 6 && grupo.getResponsavel() == null) {
                                    grupo.setResponsavel(investigador);
                                } //o grupo não pode ter 2 responsáveis
                                else if (info.length == 6 && grupo.getResponsavel() != null) {
                                    System.out.println("O grupo " + grupo.getAcronimo() + " não pode ter dois responsáveis!");
                                }
                            } else if (checkInvestigadores(investigador, listaInvestigadores())) {
                                System.out.println("Já existe um investigador com o nome " + investigador.getNome() + " ou com o endereço e-mail '" + investigador.getEmail() + "' no Cisuc.");
                            }
                            verif = true;
                        }
                    }
                    //grupo correspondente NÃO é encontrado
                    if (verif == false) {
                        System.out.println("Grupo de investigação de " + investigador.getNome() + " não existe no Cisus.");
                    }
                }
            }

            //Um estudante pode ter um orientador qualquer desde que seja efetivo
            //Ao garantir anteriormente que cada grupo tinha um responsável, garante-se que todos os grupos possuem pelo menos 1 efetivo
            //se no fim de adicionar todos os investigadores, um grupo continuar sem responsável
            Random rand = new Random(); //escolha aleatória do orientador
            int randomInd;
            for (int i = 0; i < grupos.size(); i++) {
                grupo = grupos.get(i);
                if (grupo.getResponsavel() == null) {
                    System.out.println("O grupo " + grupo.getAcronimo() + " não possui investigador responsável!");
                    grupos.remove(i); //remoção do grupo
                } else {
                    Estudante estudante;
                    //associar orientadores
                    for (int j = 0; j < grupo.getEstudantes().size(); j++) {
                        estudante = grupo.getEstudantes().get(j);
                        randomInd = rand.nextInt(grupo.getEfetivos().size());
                        estudante.setOrientador(grupo.getEfetivos().get(randomInd));
                    }
                }
            }

            System.out.println("Investigadores adicionados com SUCESSO.");
            br.close();

        } catch (FileNotFoundException ex) {
            System.out.println("Erro a abrir o ficheiro 'Investigadores.txt'!");
        } catch (IOException ex) {
            System.out.println("Erro a ler o ficheiro 'Investigadores.txt'!");
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.out.println("O ficheiro 'Investigadores.txt' não cumpre a estrutura adequada!");
        }

    }

    /**
     * recolhe informação de um ficheiro ".txt" onde estão descritas as
     * publicações
     */
    private void recolherPublicacoes() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        File f = new File("Publicações.txt");

        //if (fobj.exists() && fobj.isFile()) 
        try {
            FileReader fr = new FileReader(f);
            BufferedReader br = new BufferedReader(fr);
            String line;
            line = br.readLine(); //saltar linha do título

            line = br.readLine(); //saltar linha da legenda
            line = br.readLine(); //saltar linha em branco

            Publicacao publicacao = null;
            while ((line = br.readLine()) != null) {
                String[] info = line.split("#");
                line = br.readLine();
                String info2 = line; //resumo
                line = br.readLine();
                String[] info3 = line.split("#");
                line = br.readLine();
                String[] info4 = line.split("#"); //autores
                line = br.readLine(); //saltar linha em branco

                //Adicionar Artigos de Revista
                if (info[0].equals("AR")) {
                    publicacao = new Revista(info[1], info[2], Integer.parseInt(info[3]), Integer.parseInt(info[4]),
                            info2, info3[0], info3[1], Integer.parseInt(info3[2]));
                } //Adicionar Artigos de Conferência
                else if (info[0].equals("AC")) {
                    publicacao = new Conferencia(info[1], info[2], Integer.parseInt(info[3]), Integer.parseInt(info[4]),
                            info2, info3[0], info3[1], info3[2]);
                } //Adicionar Livros
                else if (info[0].equals("L")) {
                    publicacao = new Livro(info[1], info[2], Integer.parseInt(info[3]), Integer.parseInt(info[4]),
                            info2, info3[0], Integer.parseInt(info3[1]));
                } //Adicionar Livros de Capítulos
                else if (info[0].equals("LC")) {
                    publicacao = new Capitulos(info[1], info[2], Integer.parseInt(info[3]), Integer.parseInt(info[4]),
                            info2, info3[0], Integer.parseInt(info3[1]), info3[2], Integer.parseInt(info3[3]), Integer.parseInt(info3[4]));
                } //Adicionar Livros de Artigos de Conferência
                else if (info[0].equals("LA")) {
                    publicacao = new ArtigosConferencia(info[1], info[2], Integer.parseInt(info[3]), Integer.parseInt(info[4]),
                            info2, info3[0], Integer.parseInt(info3[1]), info3[2], Integer.parseInt(info3[3]));
                } //Tipo não aceite pelo Cisuc
                else {
                    System.out.println("Tipo de publicação '" + info[0] + "' não é aceite pelo Cisus!");
                }

                //associar autores
                boolean verif = false;
                if (grupos.isEmpty()) {
                    System.out.println("Cisus vazio!");
                } else {
                    //verificar se o ano de publicação 0,2é válido
                    if (publicacao.getAno() <= Year.now().getValue()) {
                        //verifica se já existe uma publicação como mesmo nome
                        if (checkPublicacoes(publicacao, listaPublicacoes()) == false) {
                            //percorre todos os autores mencionados para a publicação no ficheiro
                            for (int i = 0; i < info4.length; i++) {
                                for (int r = 0; r < (grupos.size()); r++) {
                                    Grupo grupo = grupos.get(r);
                                    for (int s = 0; s < (grupo.getInvestigadores().size()); s++) {
                                        Investigador investigador = grupo.getInvestigadores().get(s);

                                        //investigador(es) autor(es) encontrado(s)
                                        if (info4[i].equals(investigador.getNome())) {
                                            verif = true;

                                            investigador.getPublicacoes().add(publicacao);

                                            if (checkInvestigadores(investigador, publicacao.getAutores()) == false) {
                                                publicacao.getAutores().add(investigador);
                                            } else {
                                                System.out.println(investigador.nomeAutor() + " já está presente na lista de autores da publicação '" + publicacao.getTitulo() + "'.");
                                            }
                                        }
                                    }
                                }
                            }
                            //investigador(es) autor(es) NÃO encontrado(s)
                            if (verif == false) {
                                System.out.println("Autor(es) da publição '" + info[1] + "' não estão presentes no Cisus.");
                            }
                        } else {
                            System.out.println("Publicação '" + publicacao.getTitulo() + "' já existente no Cisuc.");
                        }
                    } else {
                        System.out.println("Ano de publicação de '" + publicacao.getTitulo() + "' é inválido.");
                    }
                }
            }
            System.out.println("Publicações adicionadas com SUCESSO.");
            br.close();

        } catch (FileNotFoundException ex) {
            System.out.println("Erro a abrir o ficheiro 'Publicações.txt'!");
        } catch (IOException ex) {
            System.out.println("Erro a ler o ficheiro 'Publicações.txt'!");
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.out.println("O ficheiro 'Publicações.txt' não cumpre a estrutura adequada!");
        }

    }

    /**
     * escreve toda a informação manipulada pelo programa num ficheiro de
     * objetos "Cisuc.obj"
     */
    private void guardar() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        File fo = new File("Cisuc.obj");

        try {
            FileOutputStream fos = new FileOutputStream(fo);
            ObjectOutputStream oos = new ObjectOutputStream(fos);

            //escrever arrayList 'grupos'
            oos.writeObject(grupos);
            oos.close();
            fos.close();

            //mensagem de sucesso
            System.out.println("\nInformação do Cisuc guardada em 'Cisuc.obj' com SUCESSO.\n");

        } catch (FileNotFoundException ex) {
            System.out.println("Erro a criar ficheiro 'Cisuc.obj'.");
        } catch (IOException ex) {
            System.out.println("Erro a escrever no ficheiro 'Cisuc.obj'.");
        }
    }

    /**
     * verifica a presença de um grupo (por nome ou acrónimo) no Cisuc
     *
     * @param g grupo a procurar
     * @return "true" de for encontrado, "false" caso contrário
     */
    private boolean checkGrupos(Grupo g) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        Grupo grupo;
        for (int i = 0; i < grupos.size(); i++) {
            grupo = grupos.get(i);
            if (grupo.getAcronimo().equals(g.getAcronimo()) || grupo.getNome().equals(g.getNome())) {
                return true;
            }
        }
        return false;
    }

    /**
     * verifica a presença de um investigador (por nome ou e-mail) num arraylist
     * de investigadores
     *
     * @param inv investigador a procurar
     * @param lista arraylist onde procurar
     * @return "true" de for encontrado, "false" caso contrário
     */
    private boolean checkInvestigadores(Investigador inv, ArrayList<Investigador> lista) { //--*--*--*--*--*--*--*--*
        Investigador investigador;
        for (int i = 0; i < lista.size(); i++) {
            investigador = lista.get(i);
            if (investigador.getNome().equals(inv.getNome()) || investigador.getEmail().equals(inv.getEmail())) {
                return true;
            }
        }
        return false;
    }

    /**
     * verifica a presença de uma publicação (por título) num arraylist de
     * publicações
     *
     * @param p publicação a procurar
     * @param lista arraylist onde procurar
     * @return "true" de for encontrado, "false" caso contrário
     */
    private boolean checkPublicacoes(Publicacao p, ArrayList<Publicacao> lista) { //--*--*--*--*--*--*--*--*--*--*--*
        Publicacao publicacao;
        for (int i = 0; i < lista.size(); i++) {
            publicacao = lista.get(i);
            if (publicacao.getTitulo().equals(p.getTitulo())) {
                return true;
            }
        }
        return false;
    }

    /**
     * verifica se uma string é constituida apenas por digitos
     *
     * @param str string a analisar
     * @return 'true' se todos os caracteres forem digitos, 'false' se pelo
     * menos um não for
     */
    public boolean checkNumeric(String str) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        for (int i = 0; i < str.length(); i++) {
            if (!Character.isDigit(str.charAt(i))) {
                return false;
            }
        }
        return true;
    }
}
