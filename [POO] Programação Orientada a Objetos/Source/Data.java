package projeto;

import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * 
 * @author Thomas Pereira Fresco | nº 2019219057
 */
public class Data implements Serializable { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

    private int dia, mes, ano;

    /**
     * Construtor de objetos do tipo "Data" com parâmetros inteiros
     * @param dia dia da data (nuemro inteiro)
     * @param mes mês da data (numero inteiro)
     * @param ano ano da data (numero inteiro)
     */
    public Data(int dia, int mes, int ano) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.ano = verifAno(ano);
        this.mes = verifMes(mes);
        this.dia = verifDia(dia);
    }

    /**
     * Construtor de objetos do tipo "Data" com parâmetro String
     * @param data data em formato String (dd/mm/aaaa)
     */
    public Data(String data) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        String[] dados = data.split("/");
        try {
            ano = verifAno(Integer.parseInt(dados[2]));
        } catch (NumberFormatException ex) {
            ano = -1;
        }
        try {
            mes = verifMes(Integer.parseInt(dados[1]));
        } catch (NumberFormatException ex) {
            mes = -1;
        }
        try {
            dia = verifDia(Integer.parseInt(dados[0]));
        } catch (NumberFormatException ex) {
            dia = -1;
        }
    }

    /**
     * Construtor de objetos do tipo "Data" com parâmetro String
     * Verifica se uma determinado data corresponde a um acontecimento passado ou futuro
     * @param data data em formato String (dd/mm/aaaa)
     * @param s "passado" para verificar se é uma data passada, "futuro" para verificar se é uma data futura
     */
    public Data(String data, String s) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        Calendar cal = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");
        String[] hoje = sdf.format(cal.getTime()).split("/");
        String[] dados = data.split("/");
        try {
            ano = verifAno(Integer.parseInt(dados[2]));
        } catch (NumberFormatException ex) {
            ano = -1;
        }
        try {
            mes = verifMes(Integer.parseInt(dados[1]));
        } catch (NumberFormatException ex) {
            mes = -1;
        }
        try {
            dia = verifDia(Integer.parseInt(dados[0]));
        } catch (NumberFormatException ex) {
            dia = -1;
        }

        //verificar data passada
        if (s.equals("passado")) {
            if (ano > Integer.parseInt(hoje[2])) {
                ano = -2;
            } else if (mes > Integer.parseInt(hoje[1]) && ano == Integer.parseInt(hoje[2])) {
                mes = -2;
            } else if (dia > Integer.parseInt(hoje[0]) && mes == Integer.parseInt(hoje[1])) {
                dia = -2;
            }
        //verificar data futura
        } else if (s.equals("futuro")) {
            if (ano < Integer.parseInt(hoje[2])) {
                ano = -3;
            } else if (mes < Integer.parseInt(hoje[1]) && ano == Integer.parseInt(hoje[2])) {
                mes = -3;
            } else if (dia < Integer.parseInt(hoje[0]) && mes == Integer.parseInt(hoje[1])) {
                dia = -3;
            }
        }
    }

    /**
     * Devolve "ano"
     * @return ano
     */
    public int getAno() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return ano;
    }

    /**
     * Devolve "mes"
     * @return mes
     */
    public int getMes() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return mes;
    }

    /**
     * Devolve "dia"
     * @return dia
     */
    public int getDia() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        return dia;
    }

    /**
     * Substitui "ano"
     * @param ano novo "ano"
     */
    public void setAno(int ano) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.ano = ano;
    }

    /**
     * Substitui "mes"
     * @param mes novo "mes"
     */
    public void setMes(int mes) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.mes = mes;
    }

    /**
     * Substitui "dia"
     * @param dia novo "dia"
     */
    public void setDia(int dia) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        this.dia = dia;
    }
    
    /**
     * Verifica se o ano cumpre as normas do calendário
     * @param ano
     * @return devolve 'ano' se cumprir os requesitos, caso contrário devolve "-1"
     */
    private int verifAno(int ano) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        //não faz sentido o ano ser negativo      
        if (ano < 0) {
            return -1;
        } else {
            return ano;
        }
    }

    /**
     * Verifica se o mês cumpre as normas do calendário
     * @param mes
     * @return devolve 'mes' se cumprir os requesitos, caso contrário devolve "-1"
     */
    private int verifMes(int mes) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        //mês tem que ser >1 e <12 
        if (mes < 1 || mes > 12) {
            return -1;
        }
        return mes;
    }

    /**
     * Verifica se o dia cumpre as normas do calendário
     * @param dia
     * @return devolve 'dia' se cumprir os requesitos, caso contrário devolve "-1"
     */
    private int verifDia(int dia) { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*

        //fevereiro com 28 dias para anos comuns
        if (mes == 2 && ano % 4 != 0) {
            if (dia < 1 || dia > 28) {
                return -1;
            }

            //fevereiro com 29 dias para anos bissextos
        } else if (mes == 2 && ano % 4 == 0) {
            if (dia < 1 || dia > 29) {
                return -1;
            }

            //outros meses
        } else {
            //meses com 30 dias
            if (mes < 7 && mes % 2 == 0) {
                if (dia < 1 || dia > 30) {
                    return -1;
                }
            } //meses com 31 dias
            else {
                if (dia < 1 || dia > 31) {
                    return -1;
                }
            }
        }
        return dia;
    }

    /**
     * organiza uma string com informação do objeto para print
     * @return string com informação do objeto para print
     */
    @Override
    public String toString() { //--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*
        if (ano == -1 && mes != -1 && dia != -1) { //ano inválido
            return "Ano Inválido!";
        } else if (ano != -1 && mes == -1 && dia != -1) { //mês inválido
            return "Mês Inválido!";
        } else if (ano != -1 && mes != -1 && dia == -1) { //dia inválido
            return "Dia Inválido!";
        } else if (ano == -1 || mes == -1 || dia == -1) { //mais que um inválido
            return "Data Inválidada!";
        } else if (ano == -2 || mes == -2 || dia == -2) { //data não é passada
            return "Data não representa acontecimento passado!";
        } else if (ano == -3 || mes == -3 || dia == -3) { //data não é futura
            return "Data não representa acontecimento futuro!";
        } else { //todos válidos
            return dia + "/" + mes + "/" + ano;
        }
    }
}
