// Created by Thomas Fresco on 05/05/2020.

#include <stdio.h>
#include <string.h>
#include "funcoes.h"

/*declaração variáveis usadas em 'main.c' -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//para uso em 'input()'
struct entidade nova_entidade; //nova entidade para inserir (medico ou doente)
char prior[MAXNOME+1] = ""; //prioridade dessa entidade (caso seja doente)

/*programa principal -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

int main() {

    //leitura dos nomes dos ficheiros de entrada e saída a partir do ficheiro config.txt
    if (leitura_n_entrada_saida()==1) return 1;

    //inserção dos dados obtidos pela leitura do ficheiro de entrada
    if (put_dados()==0) printf("Dados lidos e inseridos COM SUCESSO.\n");
    else return 1;

    //atualiza as marcações de todas as entidades e guarda no ficheiro de saida todas as alterações das listas
    guardar_ficheiro();

    //leitura da escolha (por switch) e chamada de funções consoante a opção
    while (TRUE) {

        //imprime o menu
        mostrar_menu();

        //obter opcao
        while(TRUE) {
            printf("Opção: ");
            if (leitura(opcao,MAXNOME+1)==0) break;
        }

        if (strlen(opcao)>1) strcpy(opcao,""); //salvaguarda para o switch, caso o input tenha mais que 1 char

        switch (opcao[0]) {

            case '1': //Mostrar lista médicos
                mostra_lista_medicos();
                break;
            case '2': //Mostrar lista doentes
                mostra_lista_doentes();
                break;
            case '3': //inserir médico
                if (input_entidade()==1) break;
                if(colocar_medico(nova_entidade,0)==1) {
                    printf("Erro na colocação: falta de espaço.");
                }
                else {
                    printf("%s adicionado à lista de médicos COM SUCESSO.\n",nova_entidade.nome);
                    guardar_ficheiro();
                }
                break;
            case '4': //inserir doente
                if (input_entidade()==1) break;
                if(colocar_doente(nova_entidade,str_to_int(prior))==1) {
                    printf("Erro na colocação: falta de espaço.\n");
                }
                else {
                    printf("%s adicionado à lista de doentes COM SUCESSO.\n",nova_entidade.nome);
                    guardar_ficheiro();
                }
                break;
            case '5': //remover médico
                if (retirar_medico()==0) {
                    guardar_ficheiro();
                }
                break;
            case '6': //remover doente
                if (retirar_doente()==0) {
                    guardar_ficheiro();
                }
                break;
            case '7': //procurar entidade
                printf("\nNome entidade a procurar: ");
                leitura(nome,MAXNOME+1);
                procurar_entidade(nome);
                break;
            case '8': //mostrar lista de espera
                mostra_lista_espera();
                break;
            case '9': //encerrar o programa
                return 0;
            default: //controlo de opção inválida
                printf("Opção invalida.\n");
                break;
        }
    }
}

/*funções auxiliares main.c -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//imprime o menu
void mostrar_menu() {
    printf("\n1.Mostrar lista médicos          2.Mostrar lista doentes\n"
           "3.Inserir médico                 4.Inserir doente\n"
           "5.Remover médico                 6.Remover doente\n"
           "7.Procurar entidade              8.Lista de espera\n"
           "9.Sair\n");
}

//recebe do utilizador informações sobre a entidade a adicionar
//return 1 se a entidade já se encontrar numa das listas
//return 0 se correr com sucesso
int input_entidade() {
    //aux verificar a existência da especicialidade no ficheiro
    espec=0;

    //obter nome da entidade
    printf("\nNome: ");
    leitura(nova_entidade.nome,MAXNOME+1);

    //verificar se a entidade já se encontra numa das listas
    if (procurar_entidade(nova_entidade.nome)==0) {
        printf("A entidade %s já se encontra no ficheiro.\n",nova_entidade.nome);
        return 1;
    }

    //aux verificar a existência da especicialidade no ficheiro
    espec=1;

    //obter especialidade da entidade
    printf("Especialidade: ");
    leitura(nova_entidade.especialidade,MAXNOME+1);
    if (opcao[0]=='3') {
        if (procurar_entidade(nova_entidade.especialidade)==0) {
            printf("Já se encontra no ficheiro um profissional de %s.\n",nova_entidade.especialidade);
            return 1;
        }
    }

    //obter especialidade entidade (se for doente)
    if (opcao[0]=='4') {
        while (TRUE) {
            printf("Prioridade: ");
            leitura(prior,MAXNOME+1);
            if (1<=str_to_int(prior) && str_to_int(prior)<=4) break;
            printf("%s é um valor inválido para prioridade (1 a 4).\n",prior);
        }
    }
    return 0;
}