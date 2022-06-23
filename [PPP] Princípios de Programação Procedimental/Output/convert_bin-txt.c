// Created by Thomas Fresco on 05/05/2020.

/*Este programa converte um ficheiro binário num ficheiro texto, cujos nomes são,
respetivamente, o primeiro e o segundo parâmetros passados ao correr o executável na shell.
Aconcelha-se que o primeiro parâmetro coincida com o nome que se encontra na segunda linha
do ficheiro "config.txt" (saida.bin), para que o ficheiro binário "saida.bin", produzido pelo
programa principal, possa ser descodificado para informação texto.*/

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

//converte binário para texto
void converte_bin_txt(FILE * bin,FILE * txt);

int main(int argc, char *argv[]) {
    FILE *f1 = NULL;
    FILE *f2 = NULL;

    //verificar a presença dos dois argumentos necessários
    if (verif_arg(argv[1],argv[2]) == 1) return 0;

    //verificar extensão do ficheiro de entrada (binário)
    if (verif_ext(argv[1],".bin",1) == 1) return 0;

    //verificar extensão do ficheiro de saída (texto)
    if (verif_ext(argv[2],".txt",2) == 1) return 0;

    //abertura do ficheiro binário para leitura
    f1 = fopen(argv[1], "r");
    if (erro(f1,argv[1],1)==1) return 0;

    //abertura do futuro ficheiro texto para escrita
    f2 = fopen(argv[2], "w");
    if (erro(f2,argv[2],2)==1) return 0;

    //conversão
    converte_bin_txt(f1,f2);

    //print mensagem de sucessão no ecrã
    printf("\nFicheiro binário (%s) convertido para texto (%s) COM SUCESSO.\n\n",argv[1],argv[2]);

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

void converte_bin_txt(FILE * bin,FILE * txt) {
    while (!feof(bin)) { //enquanto não chega ao EOF
        //leitura do ficheiro .bin
        fread(&num, sizeof(int), 1, bin); //sizeof(int) --> 4 bytes
        //converter o char no valor inteiro ascii
        ch = (char)num;
        //escrita no ficheiro .txt
        fwrite(&ch, sizeof(char), 1, txt); //sizeof(char) --> 1 bytes
    }
}