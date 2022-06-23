// Created by Thomas Fresco on 05/05/2020.

/*Inicialmente, porgrama recebe, de um ficheiro binário ('entrada.bin',primeira linha do ficheiro 'config.txt'),
informações sobre entidades a adicionar (16 médicos, 84 doentes).
Para que o programa receba informação, cada linha do ficheiro de entrada deve ser do tipo:

          identificador/nome_entidade/especialidade/prioridade

--> exemplo para médico: "M/joão silva/cardiologia"
--> exemplo para doente: "D/Maria Costa/dermatologia/3"
(médicos e doentes podem encontrar-se baralhados no documento.)

 Organiza a informação em duas listas: medicos e suas marcações doentes e suas marcações. Cria marcações
de consultas, cruzando a informação das duas listas. Sempre que as listas são modificadas, as marcações são
atualizadas e o ficheiro de saída ('saida.bin',segunda linha do ficheiro 'config.txt') é reescrito.

 O programa disponibiliza ainda algumas funcionalidades extra, através de um menu switch, que envolvem
informação pedida ao utilizador:

        1.Mostrar lista médicos          2.Mostrar lista doentes
        3.Inserir médico                 4.Inserir doente
        5.Remover médico                 6.Remover doente
        7.Procurar entidade              8.Lista de espera
        9.Sair
*/

#ifndef TRABALHO_FUNCOES_H
#define TRABALHO_FUNCOES_H

/*defines e variáveis-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#define TRUE 1
#define MAX 5 //outros usos
#define MAXNOME 100
#define MAXLINHA 256
#define LIM_MARCACOES 10

//recebe o valor que vai ser considerado para a seleção dos cases no menu switch em 'main()'
char opcao[MAXNOME+1];

//variável auxiliar para o case '7' do menu switch em 'main()'
char nome[MAXNOME+1];

//variável auxiliar para o case '3' do menu switch em 'main()'
int espec;

/*definição de structs -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//entidade poderá ser medico ou doente
//assumo que os médicos só podem ter até 10 marcações, das 10h às 15h (30 em 30 minutos)
//os doentes apenas poderão ter uma marcação cada um
//cada médico terá apenas uma especialidade e cada doente apenas uma doença
//não poderá existir 2 médicos com a mesma especialidade
struct entidade {
    char nome[MAXNOME+1]; //nome (do medico/doente)
    char especialidade[MAXNOME+1];//especialidade (do médico(1)/as que o doente precisa)
    char marcacoes[LIM_MARCACOES][MAXNOME+1];//marcações (do médico/doente)
};

//esta estrutura será usada para criar 2 listas: medicos e suas marcações; doentes e suas marcações.
//médicos serão inseridos pela ordem que forem adicionados
//doentes serão inseridos por ordem de prioridade (com 4 em primeiro, com 1 em último)
struct elem_lista {
    struct entidade ent;
    int prioridade;
    struct elem_lista *next;
    struct elem_lista *prev;
};

/*str to int -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//converte str para int
//argumento 1: ponteiro para array de chars para ser convertido
//return inteiro resultante da conversão
int str_to_int(char* str);

/*funções leitura -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//lé os nomes dos ficheiros binários de entrada e saída a partir do ficheiro de texto 'config.txt'
//return 1 se ocorrer erro na abertura do ficheiro 'config.txt'
//return 0 se correr com sucesso
int leitura_n_entrada_saida();

//lê de stdin
//argumento 1: array de chars onde vai ser guardada a informação lida
//argumento 2: tamanho máximo do array de chars a ler
//return 1 se ocorrer erro na leitura
//return 0 se correr com sucesso
int leitura(char string[],int tamanho);

/*funções auxiliares main.c -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//imprime o menu
void mostrar_menu();

//recebe do utilizador informações sobre a entidade a adicionar
//return 1 se o nome da entidade a inserir já se encontrar numa das listas
//return 0 se correr com sucesso
int input_entidade();

/*funções que envolvem o uso das struct -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

//lê a informação do ficheiro de entrada 'entrada.bin' e guarda-a em duas listas (médicos e doentes)
//é a primeira função a ser chamada no programa e só é chamada uma vez
//return 1 se houver erro na abertura do ficheiro de entrada
//return 0 se correr com sucesso
int put_dados();

//imprime no ecrã a lista de médicos
//return 1 se a lista estiver vazia
//return 0 se correr com sucesso
int mostra_lista_medicos();

//imprime no ecrã a lista de doentes
//return 1 se a lista estiver vazia
//return 0 se correr com sucesso
int mostra_lista_doentes();

//imprime no ecrã a 'lista' (fictícia) de espera
//return 1 se a lista estiver vazia
//return 0 se correr com sucesso
int mostra_lista_espera();

//insere entidade na lista de médicos
//argumento 1: entidade a inserir
//argumento 2: prioridade do médico (não faz sentido, é sempre 0)
//return 1 se não houver espaço para inserir uma nova struct
//return 0 se correr com sucesso
int colocar_medico(struct entidade nova_ent, int prioridade);

//insere entidade na lista de doentes
//argumento 1: entidade a inserir
//argumento 2: prioridade do doente (int de 1 a 4)
//return 1 se não houver espaço para inserir uma nova struct
//return 0 se correr com sucesso
int colocar_doente(struct entidade nova_ent, int prioridade);

//retira entidade da lista de médicos
//return 1 se a lista estiver vazia ou não existir a entidade que se pretende retirar
//return 0 se correr com sucesso
int retirar_medico();

//retira entidade da lista de doentes
//return 1 se a lista estiver vazia ou não existir a entidade que se pretende retirar
//return 0 se correr com sucesso
int retirar_doente();

//imprime no ecrã a 'lista' (fictícia) de espera
//argumento 1: array de chars correspondente ao nome da entidade a procurar
//return 1 se as duas listas estiverem vazias ou não existir a entidade que se pretende procurar
//return 0 se correr com sucesso
int procurar_entidade(char nome_procurado[MAXNOME+1]);

//atualiza as marcações de todas as entidades
//return 1 se pelo menos uma das listas estiver vazia
//return 0 se correr com sucesso
int inserir_marcacoes();

//chama inserir_marcacoes() e guarda no ficheiro de saida todas as alterações das listas
//return 1 se houver erro na abertura do ficheiro de saída
//return 0 se correr com sucesso
int guardar_ficheiro();

#endif //TRABALHO_FUNCOES_H
