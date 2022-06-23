//Filipe Ribeiro | 2019223576
//Thomas Fresco | 2019219057

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

#define SIZE 100
#define N 10

#define PIPE_NAME "/home/user/Desktop/comandos"

typedef struct {
    int id;
    pid_t idEquipa;
    char equipa;
    int numero;
    int velocidade;
    float consumo;
    int fiabilidade;
    char estado[SIZE]; //corrida, seguranca, box, desistencia, terminado
    double distPerc;
    int tempo;
    double combustivel;
    int voltas;
    int pBox;
    bool avaria;
} car;

typedef struct {
    pid_t id;
    char nome;
    char estadoBox[N]; //livre, reservada, ocupada
    int reservou; //id do carro que reservou a box
} team;

typedef struct {
    int tempo; //Número de unidades de tempo por segundo
    int distancia_volta; //Distância de cada volta em metros
    int num_voltas; //Número de voltas da corrida
    int num_equipas; //Número de equipas (mínimo 3)
    int num_max_carros; //Número máximo de carros por equipa
    int T_Avaria; //Número de unidades de tempo entre novo cálculo de avaria
    int T_Box_Min; //Tempo mínimo de reparação
    int T_Box_Max; //Tempo máximo de reparação
    int capacidade; //Capacidade do depósito de combustível em litros

    int totalAvarias;
    int totalAbast;
    int carrosPista;

    team equipas[SIZE];
    car carros[SIZE];

    bool estadoCorrida;
    bool terminar;

    int malfPid;

    int id[N];
    int countEquipas;
    int countCarros;
} Dados;

typedef struct _msgbuf {
    long mtype; 
    char mtext[100];
} msgbuf;

key_t key; // key for msgget
int msqid; // id to the msgqueue

Dados *shm;
int shmid;

sem_t * mutex_log;
sem_t * mutex_write;
sem_t * mutex_start;

pthread_t my_thread[SIZE];
pthread_mutex_t boxes[SIZE];

int fd_pipe[SIZE][2];
fd_set read_set;

void escreverLog(char mensagem[]);
int lerFicheiro();
void iniciar();
void terminar();
void teamManager();
void raceManager();
void malfunctionManager();
void handle_sigusr2(int signum);
void simulador(int pid);
void esperaVolta(int index,int team);
void sigpipe(int signum);
void sigtstp(int signum);
void sigint(int signum);
void sigterm(int signum);
void estatisticas(char flag[]);
char* substring(char str[], int start, int end);
int verifComando(char comando[]);