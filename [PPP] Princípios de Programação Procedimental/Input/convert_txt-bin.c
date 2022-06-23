// Created by Thomas Fresco on 05/05/2020.

/*Este programa converte um ficheiro texto num ficheiro binário, cujos nomes são,
respetivamente, o primeiro e o segundo parâmetros passados ao correr o executável na shell.
Aconcelha-se que o segundo parâmetro coincida com o nome que se encontra na primeira linha
do ficheiro "config.txt" (entrada.bin), para que o programa principal o possa ler*/

#include <stdio.h>
#include <string.h>
#define MAXNOME 20
#define MAXEXT 5

char ch;
int num;

//previsão de possíveis erros de abertura
int erro(FILE *p, char nome[MAXNOME+1], int n_arg);

//verificar a presença dos dois primeiros argumentos
int verif_arg(char argument1[MAXNOME+1],char argument2[MAXNOME+1]);

//verificar extensão adequada do nome do ficheiro
int verif_ext(char nome[MAXNOME+1],char extensao[MAXEXT+1], int n_arg);

//converte texto para binario
void converte_txt_bin(FILE * txt,FILE * bin);

int main(int argc, char *argv[]) {
    FILE *f1 = NULL;
    FILE *f2 = NULL;

    //verificar a presença dos dois argumentos necessários
    if (verif_arg(argv[1],argv[2]) == 1) return 0;

    //verificar extensão do ficheiro de entrada (texto)
    if (verif_ext(argv[1],".txt",1) == 1) return 0;

    //verificar extensão do ficheiro de saída (binário)
    if (verif_ext(argv[2],".bin",2) == 1) return 0;

    //abertura do ficheiro texto para leitura
    f1 = fopen(argv[1], "r");
    if (erro(f1,argv[1],1)==1) return 0;

    //abertura do futuro ficheiro binário para escrita
    f2 = fopen(argv[2], "w");
    if (erro(f2,argv[2],2)==1) return 0;

    //conversão
    converte_txt_bin(f1,f2);

    //print mensagem de sucessão no ecrã
    printf("\nFicheiro texto (%s) convertido para binário (%s) COM SUCESSO.\n\n",argv[1],argv[2]);

    //terminação
    fclose(f1);
    fclose(f2);
    return 0;
}

int erro(FILE * p,char nome[MAXNOME+1], int n_arg) {
    if (p==NULL) {
        printf("Erro na abertura do %dº ficheiro: %s\n\n",n_arg,nome);
        return 1;
    }
    else return 0;
}

int verif_ext(char nome[MAXNOME+1],char extensao[MAXEXT+1], int n_arg) {
    //strrchr() devolve ponteiro para a última ocorrência de determinado char; não existe --> NULL
    if ( (strrchr(nome, '.')==NULL) || (strcmp(strrchr(nome, '.'),extensao) != 0) ) {
        printf("Extensão do %dº ficheiro inválida: %s\n\n",n_arg,nome);
        return 1;
    }
    else return 0;
}

int verif_arg(char argument1[MAXNOME+1],char argument2[MAXNOME+1]) {
    if ((argument1==NULL) || (argument2==NULL)) {
        printf("Erro: Não foram introduzidos todos os argumentos necessários.\n\n");
        return 1;
    }
    else return 0;
}

void converte_txt_bin(FILE * txt,FILE * bin) {
    while (!feof(txt)) { //enquanto não chega ao EOF
        //leitura do ficheiro .txt
        fread(&ch, sizeof(char), 1, txt); //sizeof(char) --> 1 bytes
        //converter o char no valor inteiro ascii
        num = ch;
        //escrita no ficheiro .bin
        fwrite(&num, sizeof(int), 1, bin); //sizeof(int) --> 4 bytes
    }
}