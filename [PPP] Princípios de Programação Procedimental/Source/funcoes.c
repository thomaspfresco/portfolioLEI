// Created by Thomas Fresco on 05/05/2020.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "funcoes.h"

/*declaração variáveis usadas em 'funcoes.c' -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//ponteiros para ficheiros
FILE * f_config;
FILE * f_saida;

//array auxiliar para associação dos horários das marcações
const char horarios[LIM_MARCACOES][MAX+1] =
        {"10h00","10h30","11h00","11h30","12h00",
         "12h30","13h00","13h30","14h00","14h30"};

//nomes ficheiro de entrada e saída que vão ser lidos do ficheiro "config.txt"
char input[MAXNOME+1], output[MAXNOME+1];

//string auxiliar para int_to_str()
char str[MAX+1] = "";

/*declaração array de chars auxilares (para printf ou fwrite) -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

char header_l_medicos[] = "\n----//-----//----LISTA MÉDICOS----//-----//----\n";
char header_l_doentes[] = "\n----//-----//----LISTA DOENTES----//-----//----\n";
char header_espera[] = "\n----//-----//----LISTA DE ESPERA----//-----//----\n";
char doente_de[] = " - Doente de ";
char especialista_em[] = " - Especialista em ";
char urgencia[] =" | Nível de urgência: ";
char marcacoes[] ="Marcações:\n";
char s_marcacoes[] ="Sem marcações.\n";
char l_espera[] = "Em lista de espera.\n";

/*conversao -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//converte str para int
//argumento 1: ponteiro para array de chars para ser convertido
//return inteiro resultante da conversão
int str_to_int(char * string) {
    int res = 0;
    for (int i = 0; string[i] != '\0'; ++i) res = res * 10 + string[i] - '0';
    return res;
}

//converte int para str
//argumento 1: numero para ser convertido
//return array de chars resultante da conversão
char * int_to_str(int num) {
    char const digit[] = "0123456789";
    char* p = str;
    int i = num;
    while(i) {
        ++p;
        i = i/10;
    }
    *p = '\0';
    while(num) {
        *--p = digit[num%10];
        num = num/10;
    }
    return p;
}

/*delcaração static struct -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//Ponteiros entrada e saida para as duas listas (de médicos e de doentes)
//ponteiros para entrada e saída a 'NULL' -> listas vazias
static struct elem_lista * entrada_medicos = NULL;
static struct elem_lista * saida_medicos = NULL;
static struct elem_lista * entrada_doentes = NULL;
static struct elem_lista * saida_doentes = NULL;

/*funções leitura -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//controla erros de abertura de ficheiros
//argumento 1: ponteiro para o ficheiro que foi alvo de tentativa de abertura
//argumento 2: array de chars correnspondente ao nome do ficheiro
//return 1 se ocorrer erro na abertura
//return 0 se correr com sucesso
int erro(FILE * p,char nome_f[MAXNOME+1]) {
     if (p==NULL) {
         printf("Erro na abertura do ficheiro: %s\n\n",nome_f);
         return 1;
     }
     else return 0;
 }

//lé os nomes dos ficheiros binários de entrada e saída a partir do ficheiro de texto 'config.txt'
//return 1 se ocorrer erro na abertura do ficheiro 'config.txt'
//return 0 se correr com sucesso
int leitura_n_entrada_saida() {
    f_config = fopen("config.txt", "r");
    if (erro(f_config,"config.txt")==1) return 1;
    fscanf(f_config, "%s[^\n]", input);
    fscanf(f_config, "%s[^\n]", output);
    fclose(f_config);
    return 0;
}

//lê de stdin
//argumento 1: array de chars onde vai ser guardada a informação lida
//argumento 2: tamanho máximo do array de chars a ler
//return 1 se ocorrer erro na leitura
//return 0 se correr com sucesso
int leitura(char string[],int tamanho) {
   if (fgets(string, tamanho, stdin) == NULL) {
       printf("Erro na leitura do stdin.\n");
       return 1;
   }
   string[strlen(string)-1] = '\0';
   //printf("\n%s\n",string);
   return 0;
}

/*funções que envolvem o uso das struct -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//atualiza as marcações de todas as entidades
//return 1 se pelo menos uma das listas estiver vazia
//return 0 se correr com sucesso
int inserir_marcacoes() {
    struct elem_lista *pos_m = entrada_medicos;
    struct elem_lista *pos_d = entrada_doentes;
    char marcacao[MAXNOME+1] = "";
    char auxiliar[MAXNOME+1] = "";
    int i,count_m=0;

    //uma das listas vazia, impossível inserir marcações
    if (pos_m == NULL || pos_d == NULL) {
        printf("Não é possível atualizar marcações.\n");
        return 1;
    }

    //tornar '0' todas as marcações de todos os médicos
    while (pos_m != NULL) {
        for (int a = 0; a < LIM_MARCACOES; a++) {
            strcpy(pos_m->ent.marcacoes[a],"\0");
        }
        count_m++;
        pos_m = pos_m->next;
    }

    //percorre todos os doentes e cria as marcações
    while (pos_d != NULL) {

        pos_m = entrada_medicos;

        //verificar se há e qual o médico da mesma especialidade que a doença do doente
        for (i = 0 ; i<count_m ; i++) {
        if (strcmp(pos_m->ent.especialidade, pos_d->ent.especialidade) != 0) pos_m = pos_m->next;
        }

        i = 0;
        //não há
        if (pos_m == NULL) strcpy(pos_d->ent.marcacoes[0],"\0");
        //verificar se há espaço na agenda do médico
        else {
            for (int a=0;a<LIM_MARCACOES;a++) {
                if (strcmp(pos_m->ent.marcacoes[i], "\0") != 0) i++;
                else break;
            }
            //não há
            if (i > 9) strcpy(pos_d->ent.marcacoes[0],l_espera);
            //insersão das marcações
            else {
                strcpy(marcacao, horarios[i]);
                strcpy(auxiliar, strcat(marcacao, " | "));
                strcpy(pos_m->ent.marcacoes[i], strcat(auxiliar, pos_d->ent.nome));
                strcat(marcacao, "Dr/Drª ");
                strcpy(pos_d->ent.marcacoes[0], strcat(marcacao, pos_m->ent.nome));
            }
        }
        pos_d = pos_d->next;
    }
    printf("Marcações atualizadas COM SUCESSO.\n");
    return 0;
}

//insere entidade na lista de médicos
//argumento 1: entidade a inserir
//argumento 2: prioridade do médico (não faz sentido, é sempre 0)
//return 1 se não houver espaço para inserir uma nova struct
//return 0 se correr com sucesso
int colocar_medico(struct entidade nova_ent, int prioridade) {
    struct elem_lista *aux, *prox, *anterior;

    //Obter espaço para um novo elemento
    aux = (struct elem_lista *) malloc(sizeof(struct elem_lista));
    if (aux == NULL) return 1; //não há espaço

    //construir novo elemento
    aux->ent = nova_ent;
    aux->prioridade = prioridade;
    aux->next = NULL;
    aux->prev = NULL;

    //procurar a posição onde a entidade deve ficar
    if (entrada_medicos == NULL) {
        //lista vazia, é a primeira entidade
        entrada_medicos = aux;
        saida_medicos = aux;
    } else {
        //lista contém entidades
        if (entrada_medicos->prioridade <= prioridade) {
            // inserir à entrada da lista
            entrada_medicos->prev = aux;
            aux->next = entrada_medicos;
            entrada_medicos = aux;
        } else {
            //procurar posição de inserção
            anterior = entrada_medicos;
            prox = entrada_medicos->next;
            while (prox != NULL && prox->prioridade < prioridade) {
                anterior = prox;
                prox = prox->next;
            }
            if (prox == NULL) {
                // inserir à saída da lista
                anterior->next = aux;
                aux->prev = anterior;
                saida_medicos = aux;
            } else {
                // inserir a meio da lista
                anterior->next = aux;
                aux->prev = anterior;
                aux->next = prox;
                prox->prev = aux;

            }
        }
    }
    return 0;
}

//insere entidade na lista de doentes
//argumento 1: entidade a inserir
//argumento 2: prioridade do doente (int de 1 a 4)
//return 1 se não houver espaço para inserir uma nova struct
//return 0 se correr com sucesso
int colocar_doente(struct entidade nova_ent, int prioridade) {
    struct elem_lista *aux, *prox, *anterior;

    //Obter espaço para um novo elemento
    aux = (struct elem_lista *) malloc(sizeof(struct elem_lista));
    if (aux == NULL) return 1; //não há espaço

    //construir novo elemento
    aux->ent = nova_ent;
    aux->prioridade = prioridade;
    aux->next = NULL;
    aux->prev = NULL;

    //procurar a posição onde a entidade deve ficar
    if (entrada_doentes == NULL) {
        //lista vazia, é a primeira entidade
        entrada_doentes = aux;
        saida_doentes = aux;
    } else {
        //lista contém entidades
        if (entrada_doentes->prioridade <= prioridade) {
            // inserir à entrada da lista
            entrada_doentes->prev = aux;
            aux->next = entrada_doentes;
            entrada_doentes = aux;
        } else {
            //procurar posição de inserção
            anterior = entrada_doentes;
            prox = entrada_doentes->next;
            while (prox != NULL && prox->prioridade > prioridade) {
                anterior = prox;
                prox = prox->next;
            }
            if (prox == NULL) {
                // inserir à saída da lista
                anterior->next = aux;
                aux->prev = anterior;
                saida_doentes = aux;
            } else {
                // inserir a meio da lista
                anterior->next = aux;
                aux->prev = anterior;
                aux->next = prox;
                prox->prev = aux;

            }
        }
    }
    return 0;
}

//retira entidade da lista de médicos
//return 1 se a lista estiver vazia ou não existir a entidade que se pretende retirar
//return 0 se correr com sucesso
int retirar_medico() {
    struct elem_lista * no_a_retirar = entrada_medicos;

    // lista vazia
    if (entrada_medicos == NULL) {
        printf("Lista de médicos vazia.\n");
        return 1;
    }

    //leitura do nome da entidade a retirar
    int i=0;
    char r_nome[MAXNOME+1]="";
    printf("\nNome do médico a retirar: ");
    while((r_nome[i] = (char)getchar())!='\n') i++;
    r_nome[strlen(r_nome)-1] = '\0';

    //procurar na lista dos médicos
    while (strcasecmp(no_a_retirar->ent.nome,r_nome)!=0) {
        no_a_retirar = no_a_retirar->next;
        //nome não encontrado
        if (no_a_retirar == NULL) {
            printf("Profissional %s não encontrado.\n",r_nome);
            return 1;
        }
    }

    //só havia uma entidade na lista e vai ficar vazia
    if (no_a_retirar->prev == NULL && no_a_retirar->next == NULL) {
        free(no_a_retirar);
        entrada_medicos = NULL;
        saida_medicos = NULL;
    }
        // elemento a retirar é o primeiro
    else if (no_a_retirar->prev == NULL) {
        entrada_medicos = no_a_retirar->next;
        entrada_medicos->prev = NULL;
        entrada_medicos->next = no_a_retirar->next->next;
        free(no_a_retirar);
    }
        // elemento a retirar é o último
    else if (no_a_retirar->next == NULL) {
        //printf("|%s|",no_a_retirar->ent.nome);
        saida_medicos = no_a_retirar->prev;
        saida_medicos->prev = no_a_retirar->prev->prev;
        saida_medicos->next = NULL;
        free(no_a_retirar);
    }
        // no meio
    else {
        no_a_retirar->prev->next = no_a_retirar->next;
        no_a_retirar->next->prev = no_a_retirar->prev;
        free(no_a_retirar);
    }
    //mensagem de sucesso
    printf("Profissional %s retirado COM SUCESSO.\n",r_nome);
    return 0;
}

//retira entidade da lista de doentes
//return 1 se a lista estiver vazia ou não existir a entidade que se pretende retirar
//return 0 se correr com sucesso
int retirar_doente() {
    struct elem_lista * no_a_retirar = entrada_doentes;

    // lista vazia
    if (entrada_doentes == NULL) {
        printf("Lista de doentes vazia.\n");
        return 1;
    }

    //leitura do nome da entidade a retirar
    int i=0;
    char r_nome[MAXNOME+1]="";
    printf("\nNome do doente a retirar: ");
    while((r_nome[i] = (char)getchar())!='\n') i++;
    r_nome[strlen(r_nome)-1] = '\0';

    //procurar na lista dos doentes
    while (strcasecmp(no_a_retirar->ent.nome,r_nome)!=0) {
        no_a_retirar = no_a_retirar->next;
        //nome não encontrado
        if (no_a_retirar == NULL) {
            printf("Doente %s não encontrado.\n",r_nome);
            return 1;
        }
    }

    //só havia uma entidade na lista e vai ficar vazia
    if (no_a_retirar->prev == NULL && no_a_retirar->next == NULL) {
        free(no_a_retirar);
        entrada_doentes = NULL;
        saida_doentes = NULL;
    }
        // elemento a retirar é o primeiro
    else if (no_a_retirar->prev == NULL) {
        entrada_doentes = no_a_retirar->next;
        entrada_doentes->prev = NULL;
        entrada_doentes->next = no_a_retirar->next->next;
        free(no_a_retirar);
    }
        // elemento a retirar é o último
    else if (no_a_retirar->next == NULL) {
        //printf("|%s|",no_a_retirar->ent.nome);
        saida_doentes = no_a_retirar->prev;
        saida_doentes->prev = no_a_retirar->prev->prev;
        saida_doentes->next = NULL;
        free(no_a_retirar);
    }
        // no meio
    else {
        no_a_retirar->prev->next = no_a_retirar->next;
        no_a_retirar->next->prev = no_a_retirar->prev;
        free(no_a_retirar);
    }
    printf("Doente %s retirado COM SUCESSO.\n",r_nome);
    return 0;
}

//função auxiliar das funções 'mostra_lista'; verifica se as listas estão vazias para as poder mostrar
//argumento 1: ponteiro para a entrada da lista
//return 1 se a lista estiver vazia
//return 0 se tiver pelo menos um elemento
int aux_mostra(struct elem_lista *pos) {
    if (pos == NULL) {
        printf("Lista vazia.\n");
        return 1;
    }
   return 0;
}

//imprime no ecrã a lista de médicos
//return 1 se a lista estiver vazia
//return 0 se correr com sucesso
int mostra_lista_medicos() {
    struct elem_lista *pos = entrada_medicos;
    if (aux_mostra(pos)==1) return 1;
    printf("%s",header_l_medicos);
    while (pos != NULL) {
        printf("%s%s%s\n", pos->ent.nome,especialista_em,pos->ent.especialidade);
        pos = pos->next;
    }
    return 0;
}

//imprime no ecrã a lista de doentes
//return 1 se a lista estiver vazia
//return 0 se correr com sucesso
int mostra_lista_doentes() {
    struct elem_lista *pos = entrada_doentes;
    if (aux_mostra(pos)==1) return 1;
    printf("%s",header_l_doentes);
    while (pos != NULL) {
        printf("%s%s%s%s%d\n",pos->ent.nome,doente_de,pos->ent.especialidade,urgencia,pos->prioridade);
        pos = pos->next;
    }
    return 0;
}

//imprime no ecrã a 'lista' (fictícia) de espera
//return 1 se a lista estiver vazia
//return 0 se correr com sucesso
int mostra_lista_espera() {
    struct elem_lista *pos = entrada_doentes;
    if (aux_mostra(pos)==1) return 1;
    printf("%s",header_espera);
    while (pos != NULL) {
        if ((strcmp(pos->ent.marcacoes[0],l_espera)==0)) printf("%s%s%s%s%d\n",pos->ent.nome,doente_de,pos->ent.especialidade,urgencia,pos->prioridade);
        pos = pos->next;
    }
    return 0;
}

//imprime no ecrã a 'lista' (fictícia) de espera
//argumento 1: array de chars correspondente ao nome da entidade a procurar
//return 1 se as duas listas estiverem vazias ou não existir a entidade que se pretende procurar
//ou, no caso de ser médico, já existir um profissional com a especialidade introduzida
//return 0 se correr com sucesso
int procurar_entidade(char nome_procurado[MAXNOME+1]) {
    struct elem_lista *pos1 = entrada_medicos;
    struct elem_lista *pos2 = entrada_doentes;
    int verif=0; //verifica se a entidade identificada é médico ou doente

    //lista de médicos e de doentes vazias
    if (pos1 == NULL && pos2 == NULL) {
        printf("As duas listas estão vazias.\n");
        return 1;
    }

    if (opcao[0]=='3' && espec==1) {
        //procurar especialidade na lista dos médicos
        while (strcasecmp(pos1->ent.especialidade, nome_procurado) != 0) {
            pos1 = pos1->next;
            if (pos1 == NULL) return 1;
        }
    } else {
        //procurar na lista dos médicos
        while (strcasecmp(pos1->ent.nome, nome_procurado) != 0) {
            pos1 = pos1->next;
            if (pos1 == NULL) {
                verif = 1;
                break;
            }
        }

        //procurar na lista dos doentes
        while (strcasecmp(pos2->ent.nome, nome_procurado) != 0) {
            pos2 = pos2->next;
            if (pos2 == NULL) break;
        }

        //entidade não é encontrada
        if (pos1 == NULL && pos2 == NULL) {
            if (opcao[0] == '7') printf("Não foi encontrada a entidade com o nome '%s'.\n", nome_procurado);
            return 1;
        }
    }

    //imprime no ecrã caso a opção no switch seja '7'
    if (opcao[0]=='7') {
        //médico
        if (verif == 0) {
            printf("%s%s%s\n", pos1->ent.nome, especialista_em, pos1->ent.especialidade);
            if (strcmp(pos1->ent.marcacoes[0], "\0") != 0) printf("%s", marcacoes);
            else printf("%s", s_marcacoes);
            for (int a = 0; a < LIM_MARCACOES; a++) {
                if (strcmp(pos1->ent.marcacoes[a], "\0") != 0) printf("%s\n", pos1->ent.marcacoes[a]);
            }
            //doente
        } else {
            printf("%s%s%s\n", pos2->ent.nome, doente_de, pos2->ent.especialidade);
            if ((strcmp(pos2->ent.marcacoes[0], l_espera) == 0)) printf("%s", l_espera);
            if (strcmp(pos2->ent.marcacoes[0], "\0") != 0) {
                printf("%s", marcacoes);
                for (int a = 0; a < LIM_MARCACOES; a++) {
                    if (strcmp(pos2->ent.marcacoes[a], "\0") != 0) printf("%s\n", pos2->ent.marcacoes[a]);
                }
            } else printf("%s", s_marcacoes);
        }
    }
    return 0;
}

//lê a informação do ficheiro de entrada 'entrada.bin' e guarda-a inicialmente em duas listas (médicos e doentes)
//é a primeira função a ser chamada no programa e só é chamada uma vez
//return 1 se houver erro na abertura do ficheiro de entrada
//return 0 se correr com sucesso
int put_dados() {
    FILE * f_entrada;
    f_entrada = fopen(input,"r");
    if (erro(f_entrada,input)==1) return 1;

    while(!feof(f_entrada)) {
        struct entidade nova_ent; //nova entidade
        char ch,linha[MAXLINHA + 1] = ""; //linha a ser lida
        const char barra[2] = "/"; //caracter de separação
        int num=0,i=0,prior = 0;

        //leitura
        while(num!='\n' && !feof(f_entrada)) {
            fread(&num, sizeof(int), 1, f_entrada);
            ch = (char)num;
            linha[i]=ch;
            i++;
        }
        if(!feof(f_entrada)) linha[strlen(linha)-1] = '\0';

        //separação da informação da linha por '/'
        char *dados_ent = strtok(linha, barra);
        if(dados_ent == NULL) break;

        //marcações a 0
        for (int a = 0; a < LIM_MARCACOES; a++) {
            strcpy(nova_ent.marcacoes[a],"\0");
        }

        //é médico
        if (strcmp(dados_ent, "M") == 0) {
            dados_ent = strtok(NULL, barra);
            strcpy(nova_ent.nome, dados_ent);
            dados_ent = strtok(NULL, barra);
            strcpy(nova_ent.especialidade, dados_ent);
            colocar_medico(nova_ent, prior);
        }
            //é doente
        else {
            dados_ent = strtok(NULL, barra);
            strcpy(nova_ent.nome, dados_ent);
            dados_ent = strtok(NULL, barra);
            strcpy(nova_ent.especialidade, dados_ent);
            dados_ent = strtok(NULL, barra);
            prior = str_to_int(dados_ent);
            colocar_doente(nova_ent, prior);
        }
    }
    fclose(f_entrada);
    return 0;
}

//função auxiliar de 'aux_guardar(struct elem_lista *ptr,FILE * p)': escreve efetivamente no ficheiro
//argumento 1: array de chars correspondente à informação a ser guardada no ficheiro de saída
//argumento 2: ponteiro para o ficheiro de saída
void aux_escrita(char str_f[],FILE * p) {
    int num;
    char ch;
    //printf("\n%s\n",str_f);
    for (int i=0;i<strlen(str_f);i++) { //enquanto não chega a \0
        ch = str_f[i];
        num = ch; //conversão para ascii
        fwrite(&num, sizeof(int), 1, p); //sizeof(int) --> 4 bytes
        //printf("%c",ch);
    }
    //printf("\n");
}

//função auxiliar de 'guardar_ficheiro()': determina a ordem de escrita e manda imprimir no ficheiro de saída binário
//através da chamada da função 'aux_escrita(char str_f[],FILE * p)'.
//argumento 1: ponteiro para a entrada de uma lista
//argumento 2: ponteiro para o ficheiro de saída
void aux_guardar(struct elem_lista *ptr,FILE * p) {
    int verif = 0;
    if(ptr==entrada_doentes) verif = 1;

    //escrita título lista
    if (verif==1) aux_escrita(header_l_doentes,p);
    else aux_escrita(header_l_medicos,p);

    while (ptr != NULL) {
        //escrita nome
        aux_escrita(ptr->ent.nome,p);

        //escrita especialidade
        if (verif == 1) aux_escrita(doente_de,p);
        else aux_escrita(especialista_em,p);
        aux_escrita(ptr->ent.especialidade,p);

        //escrita prioridade (se for doente)
        if (verif == 1) {
            aux_escrita(urgencia,p);
            //fwrite(&ptr->prioridade, sizeof(int), 1, p); //sizeof(int) --> 4 bytes
            aux_escrita(int_to_str(ptr->prioridade),p);
        }
        aux_escrita("\n",p);

        //escrita marcações
        if (strcmp(ptr->ent.marcacoes[0],l_espera) == 0) aux_escrita(l_espera,p);
        if (strcmp(ptr->ent.marcacoes[0], "\0") == 0) aux_escrita(s_marcacoes,p);
        else if ((strcmp(ptr->ent.marcacoes[0],l_espera) != 0) ){
            aux_escrita(marcacoes, p);
            for (int a = 0; a < LIM_MARCACOES; a++) {
                if (strcmp(ptr->ent.marcacoes[a], "\0") != 0) {
                    aux_escrita(ptr->ent.marcacoes[a], p);
                    aux_escrita("\n", p);
                }
            }
        }
        aux_escrita("\n",p);
        ptr=ptr->next;
    }
}

//chama inserir_marcacoes() e guarda no ficheiro de saida todas as alterações das listas
//return 1 se houver erro na abertura do ficheiro de saída
//return 0 se correr com sucesso
int guardar_ficheiro() {
    struct elem_lista *pos1 = entrada_medicos;
    struct elem_lista *pos2 = entrada_doentes;

    inserir_marcacoes();

    //abertura para escrita do ficheiro de saida
    f_saida = fopen(output,"wb");
    if (erro(f_saida,output)==1) return 1;

    //escrita lista médicos
    aux_guardar(pos1,f_saida);

    //escrita lista doentes
    aux_guardar(pos2,f_saida);

    //escrita lista espera
    aux_escrita(header_espera,f_saida);
    while (pos2 != NULL) {
        if ((strcmp(pos2->ent.marcacoes[0],l_espera)==0)) {
            aux_escrita(pos2->ent.nome,f_saida);
            aux_escrita(doente_de,f_saida);
            aux_escrita(pos2->ent.especialidade,f_saida);
            aux_escrita(urgencia,f_saida);
            aux_escrita(int_to_str(pos2->prioridade),f_saida);
            aux_escrita("\n",f_saida);
        }
        pos2 = pos2->next;
    }

    fclose(f_saida);
    return 0;
}