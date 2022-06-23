//Filipe Ribeiro | 2019223576
//Thomas Fresco | 2019219057

#include "header.h"

void simulador(int pid) {

  int fd;

  signal(SIGINT,SIG_IGN);
  signal(SIGTSTP,SIG_IGN);

  sem_wait(mutex_write);
  shm->terminar = false;
  sem_post(mutex_write);

  if (fork() == 0) {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    raceManager();
  } 

  //para receber o sinal sigusr2
  signal(SIGUSR2,handle_sigusr2);
  signal(SIGTERM,sigterm);

  if ((fd = open(PIPE_NAME, O_WRONLY)) < 0) {
    perror("Cannot open pipe for writing: ");
    exit(0);
  }

  sem_wait(mutex_start);

    signal(SIGINT, sigint);
    signal(SIGTSTP, sigtstp);
}

int main() { //--------------------------------------------------------------

  int main_pid = getpid();

  iniciar();
        
  if (lerFicheiro() == 1) return 0;  
  
  while(1) {
  simulador(main_pid);

  for (int i=0;i<2;i++) wait(NULL);

  estatisticas("final");
  }

  terminar();

  return 0;
}