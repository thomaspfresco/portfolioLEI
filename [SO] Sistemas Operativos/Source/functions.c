//Filipe Ribeiro | 2019223576
//Thomas Fresco | 2019219057

#include "header.h"

void handle_sigusr1(int signum) {
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGUSR1,SIG_IGN);

    sem_wait(mutex_write);
    shm->terminar = true;
    sem_post(mutex_write);

    sem_wait(mutex_log);
    escreverLog("SIGNAL SIGUSR1 RECEIVED");
    sem_post(mutex_log);
}

void handle_sigusr2(int signum) {

    sem_wait(mutex_log);
    escreverLog("SIGNAL SIGUSR2 RECEIVED");
    sem_post(mutex_log);

    if (fork() == 0) {
      signal(SIGINT, SIG_IGN);
      signal(SIGTSTP, SIG_IGN);
      malfunctionManager();
    }
}

void sigterm(int signum) {
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGUSR1,SIG_IGN);

    sem_wait(mutex_log);
    escreverLog("SIGNAL SIGTERM RECEIVED");
    sem_post(mutex_log);

    for (int i=0;i<2;i++) wait(NULL);

    estatisticas("final");

    terminar();
    exit(0);
}


void sigtstp(int signum) { //-----------------------------------------------------
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);

    sem_wait(mutex_log);
    escreverLog("SIGNAL SIGTSTP RECEIVED");
    sem_post(mutex_log);

    estatisticas("");

    signal(SIGINT,sigint);
    signal(SIGTSTP, sigtstp);
}

void sigint(int signum) { //-----------------------------------------------------
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);

    sem_wait(mutex_write);
    shm->terminar = true;
    sem_post(mutex_write);

    sem_wait(mutex_log);
    escreverLog("SIGNAL SIGINT RECEIVED");
    sem_post(mutex_log);
    
    for (int i=0;i<2;i++) wait(NULL);

    estatisticas("final");

    terminar();
    exit(0);
}

int md_comparator(const void *v1, const void *v2) {
    const car *p1 = (car *)v1;
    const car *p2 = (car *)v2;
    if (p1->distPerc > p2->distPerc)
        return -1;
    else if (p1->distPerc == p2->distPerc)
    {
        if (p1->tempo < p2->tempo)
            return -1;
        else if (p1->tempo > p2->tempo)
            return 1;
        else
            return 0;
    }
    else
        return 1;
}

void estatisticas(char flag[]) {

    car carros[shm->countCarros];

    for (int i = 0; i<shm->countCarros; i++) carros[i] = shm->carros[i];

    qsort(carros, sizeof(carros) / sizeof(carros[0]), sizeof(carros[0]), md_comparator);

    if (strcmp(flag,"final") == 0) {
        char msg[SIZE] = "\0";
        sprintf(msg, "CAR %d WINS THE RACE", carros[0].numero);

        sem_wait(mutex_log);
        escreverLog(msg);
        sem_post(mutex_log);
    }

    printf("\n----//----/ STATISTICS /----//----\n");
    printf("=> TOP 5 <=\n");

    if (shm->countCarros < 5) {
        for (int i = 0; i<shm->countCarros; i++) printf("%d | CAR: %d, TEAM: %c, LAPS: %d, PITSTOPS: %d\n",i+1,carros[i].numero,
        carros[i].equipa,carros[i].voltas,carros[i].pBox);
    }

    else {
    for (int i = 0; i<5; i++) printf("%d | CAR: %d, TEAM: %c, LAPS: %d, PITSTOPS: %d\n",i+1,carros[i].numero,
        carros[i].equipa,carros[i].voltas,carros[i].pBox);
    }

    printf("=> LAST CAR: %d\n", carros[shm->countCarros-1].numero);
    printf("=> MALFUNCTIONS: %d\n", shm -> totalAvarias);
    printf("=> REFILLS: %d\n", shm -> totalAbast);
    printf("=> CARS RACING: %d\n", shm -> carrosPista);
    printf("----//----//----//----//----//----\n\n");
}

char* substring(char str[], int start, int end) {
    int i, j;
    char *sub; 
     
    // Verifica valores incompatíveis e retorna NULL
    if(start >= end || end > strlen(str)) {
        return NULL;
    }
     
    // Aloca memória para a substring
    sub = (char *) malloc(sizeof(char) * (end - start + 1));
     
    // Copia a substring para a variável
    for(i = start, j = 0; i < end; i++, j++) {
        sub[j] = str[i];
    }
     
    // Terminador de string
    sub[j] = '\0';
     
    return sub;
}

int verifComando(char comando[]) {
    char sub[SIZE];
    char t;
    int num,fiabilidade,speed;
    float consumo;
    bool flag;
    int count = 0;

    //verificar se todas as equipas têm pelo menos um carro
    if (strcmp(comando,"START RACE!") == 0) {
        
        sem_wait(mutex_log);
        escreverLog("COMMAND RECEIVED => START RACE!");
        sem_post(mutex_log);
        
        for (int i = 0; i<shm->countEquipas; i++) {
        t = shm -> equipas[i].nome;
            for (int j = 0; j<shm->countCarros; j++) {
                if (t == shm->carros[j].equipa) {
                    count++;
                    break;
                }        
            }
        }
        if(count == shm->num_equipas) return 1;
        //nem todas as equipas têm pelo menos um carro
        else return -5;
    }

    if (strlen(comando) < 70) return -1;

    strcpy(sub,substring(comando, 0, 13));

    if (strcmp(sub,"ADDCAR TEAM: ") != 0) return -1;

    t = comando[13];

    strcpy(sub,substring(comando, 14, 21));

    if (strcmp(sub,", CAR: ") != 0) return -1;

    if (isdigit(comando[21]) == 0 || isdigit(comando[22]) == 0) return -1;

    num = ((int)comando[21]-48)*10 + ((int)comando[22]-48);

    strcpy(sub,substring(comando, 23, 32));

    if (strcmp(sub,", SPEED: ") != 0) return -1;

    if (isdigit(comando[32]) == 0 || isdigit(comando[33]) == 0) return -1;

    speed = ((int)comando[32]-48)*10 + ((int)comando[33]-48);

    strcpy(sub,substring(comando, 34, 49));

    if (strcmp(sub,", CONSUMPTION: ") != 0) return -1;

    if (isdigit(comando[49]) == 0 || isdigit(comando[51]) == 0 || isdigit(comando[52]) == 0 || comando[50] != '.') return -1;

    consumo = ((int)comando[49]-48) + ((int)comando[51]-48)/10.0 + ((int)comando[52]-48)/100.0;

    strcpy(sub,substring(comando, 53, 68));

    if (strcmp(sub,", RELIABILITY: ") != 0) return -1;

    if (isdigit(comando[68]) == 0 || isdigit(comando[69]) == 0) return -1;

    fiabilidade = ((int)comando[68]-48)*10 + ((int)comando[69]-48);

    flag = false;

    for (int i = 0; i<shm->countEquipas; i++) {
        if (shm -> equipas[i].nome == t) {
            flag = true;
            break;
        }
    }

    //não é possivel criar mais equipas
    if ((flag == false) && (shm->countEquipas == shm->num_equipas)) return -2;

    else if ((flag == false) && (shm->countEquipas != shm->num_equipas)) {
        team equipa;

        equipa.nome = t;
        equipa.reservou = -1;
        strcpy(equipa.estadoBox,"livre");
        sem_wait(mutex_write);
        shm->equipas[shm->countEquipas] = equipa;
        shm->countEquipas++;

        sem_post(mutex_write);
    }

    flag = false;

    for (int i = 0; i<shm->countCarros; i++) {
        if (shm -> carros[i].numero == num) {
            flag = true;
            break;
        }
    }

    //o numero do carro inserido não é único
    if (flag == true) return -3;

    count = 0;

    for (int i = 0; i<shm->countCarros; i++) {
        if (shm -> carros[i].equipa == t) {
            count++;
        }
        //o numero do carro por equipa foi excedido
        if (count>=shm -> num_max_carros) return -4;
    }

    car carro;
    carro.equipa = t;
    carro.numero = num;
    carro.velocidade = speed;
    carro.consumo = consumo;
    carro.fiabilidade = fiabilidade;
    carro.combustivel = shm -> capacidade;
    carro.distPerc = 0;
    carro.voltas = 0;
    carro.pBox = 0;
    carro.tempo = 0;

    sem_wait(mutex_write);
    shm->carros[shm->countCarros] = carro;
    shm->countCarros++;
    sem_post(mutex_write);
    
    return 0;
}

void escreverLog(char mensagem[]) { //---------------------------------------
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    FILE *fp;

    fp = fopen("log.txt","a");

    printf("%d:%d:%d %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, mensagem);
    fprintf(fp, "%d:%d:%d %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, mensagem);
    fclose(fp);
}

int lerFicheiro() { //-------------------------------------------------------

    FILE *fich;

    char Linha[SIZE];
    int i = 0;
    int config[9];
    char *token;
    char s[3] = ", ";

    // Abertura do ficheiro "input.txt" para leitura
    fich = fopen("input.txt", "r");

    // ficheiro não encontrado
    if (fich == NULL) {
        sem_wait(mutex_log);
        escreverLog("FILE 'input.txt' NOT FOUND");
        sem_post(mutex_log);
        return 1;
    }

    while (fgets(Linha, SIZE, fich) != NULL) {
        token = strtok(Linha,s);

        while( token != NULL ) {
            config[i] = strtol(token, &token, 10);
            token = strtok(NULL, s);
            i++;
        }
    }

    //Verificação do mínimo de 3 equipas
    if (config[3]<3) {
        sem_wait(mutex_log);
        escreverLog("INVALID NUMBER OF TEAMS");
        sem_post(mutex_log);
        return 1;
    }
    sem_wait(mutex_write);
    shm -> tempo = config[0];
    shm -> distancia_volta = config[1];
    shm -> num_voltas = config[2];
    shm -> num_equipas = config[3];
    shm -> num_max_carros = config[4];
    shm -> T_Avaria = config[5];
    shm -> T_Box_Min = config[6];
    shm -> T_Box_Max = config[7];
    shm -> capacidade = config[8];
    shm -> countCarros = 0;
    shm -> countEquipas = 0;
    shm -> totalAvarias = 0;
    shm -> totalAbast = 0;
    sem_post(mutex_write);

    fclose(fich);
    return 0;
}

void iniciar() { //------------------------------------------------------

    //remove ficheiro log, caso exista
    remove("log.txt");

    //criar shared memory
    if ((shmid = shmget(IPC_PRIVATE, sizeof(Dados), IPC_CREAT | 0700)) < 0) {
        perror("Error in shmget with IPC_CREAT\n");
        exit(1);
    }

    // Attach shared memory
    if ((shm = (Dados *) shmat(shmid, NULL, 0)) == (Dados *)-1) {
        perror("Shmat error.\n");
        exit(1);
    }

    //Criar o named pipe se ainda nao existir
    if ((mkfifo(PIPE_NAME, O_CREAT|O_EXCL|0600)<0) && (errno!= EEXIST)) { 
        perror("Cannot create pipe: ");
        exit(0);
    }

    sem_unlink("MUTEX_LOG");
    mutex_log = sem_open("MUTEX_LOG",O_CREAT|O_EXCL,0700,1);

    sem_unlink("MUTEX_WRITE");
    mutex_write = sem_open("MUTEX_WRITE",O_CREAT|O_EXCL,0700,1);

    sem_unlink("MUTEX_START");
    mutex_start = sem_open("MUTEX_START",O_CREAT|O_EXCL,0700,1);

    
    for (int i = 0; i<shm -> num_equipas; i++) {
        if (pthread_mutex_init(&boxes[i], NULL) != 0) {
            printf("\n mutex init failed\n");
            exit(1);
        }

    }

    // chave para a MQ
    key = 1234;

    // cria a MQ
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        fprintf(stderr,"msgget error: %s\n",strerror(errno));
        exit(1);
    }
    
    sem_wait(mutex_log);
    escreverLog("SIMULATOR STARTING");
    sem_post(mutex_log);
}

void terminar() {

    sem_wait(mutex_log);
    escreverLog("SIMULATOR CLOSING");
    sem_post(mutex_log);

    sem_close(mutex_log);
    sem_unlink("MUTEX_LOG");
    sem_close(mutex_write);
    sem_unlink("MUTEX_WRITE");
    sem_close(mutex_start);
    sem_unlink("MUTEX_START");

    for (int i = 0; i < shm -> num_equipas; i++) {
        pthread_mutex_destroy(&boxes[i]);
    }

    unlink(PIPE_NAME);
    shmdt(shm);
    shmctl(shmid,IPC_RMID,NULL);
    msgctl(msqid, IPC_RMID, 0);
    }

void inBox(int index,int team,char flag[]) {
    pthread_mutex_lock(&boxes[team]);
    
    char msg[SIZE] = "\0";
    int r;

    double aux1 = ((4*shm->distancia_volta)/shm -> carros[index].velocidade)*shm -> carros[index].consumo;

    //box fica ocupada
    sem_wait(mutex_write);
    strcpy(shm -> equipas[team].estadoBox,"ocupada");
    sem_post(mutex_write);

    /*sprintf(msg, "BOX of TEAM %c BUSY", shm -> equipas[team].nome);

    sem_wait(mutex_log);
    escreverLog(msg);
    sem_post(mutex_log);*/

    sem_wait(mutex_write);
    shm -> carros[index].pBox++;
    strcpy(shm -> carros[index].estado,"box"); 
    sem_post(mutex_write);

    write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));

    sprintf(msg, "CAR %d NOW ON BOX STATE (%s)", shm -> carros[index].numero,flag);

    sem_wait(mutex_log);
    escreverLog(msg);
    sem_post(mutex_log);
    
    //reparar
    if (strcmp(flag, "REPAIR") == 0) {
        
        r = rand()%(shm->T_Box_Max-shm->T_Box_Min+1) + (shm->T_Box_Min);
        
        sleep((1/shm -> tempo)*r);

        sem_wait(mutex_write);
        shm -> carros[index].tempo += r;
        shm ->carros[index].avaria = false;
        sem_post(mutex_write);

        if (shm->carros[index].combustivel < aux1) {
            sleep((1/shm -> tempo)*2);
            sem_wait(mutex_write);
            shm -> carros[index].combustivel = shm -> capacidade;
            shm -> totalAbast++;
            shm -> carros[index].tempo += 2;
            sem_post(mutex_write);
        }
    
    //abastecer
    } else {
        sleep((1/shm -> tempo)*2);
        sem_wait(mutex_write);
        shm -> carros[index].combustivel = shm -> capacidade;
        shm -> totalAbast++;
        shm -> carros[index].tempo += 2;
        sem_post(mutex_write);
    }
    pthread_mutex_unlock(&boxes[team]);

    if (shm->terminar == true) esperaVolta(index,team);
}

void *carro(void* idp) {
    //pthread_mutex_lock(&shm->mutex);
    int my_id = *((int *)idp);
    msgbuf notificacao;
    char msg[SIZE] = "\0";
    int index, team;

    //combustivel necessario para 4 voltas
    double aux1 = ((4*shm->distancia_volta)/shm -> carros[index].velocidade)*shm -> carros[index].consumo;
    //combustivel necessario para 2 voltas
    double aux2 = ((2*shm->distancia_volta)/shm -> carros[index].velocidade)*shm -> carros[index].consumo;
    
    //determinar indice do carro
    for (int i = 0; i<shm -> countCarros; i++) {
        if (shm -> carros[i].id == my_id) {
            index = i;
            break;
        }
    }

    sem_wait(mutex_write);
    strcpy(shm -> carros[index].estado,"corrida");
    shm->carrosPista++;
    sem_post(mutex_write);

    write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));

    sprintf(msg, "CAR %d NOW ON RACE STATE", shm -> carros[index].numero);

    sem_wait(mutex_log);
    escreverLog(msg);
    sem_post(mutex_log);
    
    //determinar indice da equipa
    for (int i = 0; i<shm -> countEquipas; i++) {
        if (shm -> equipas[i].nome == shm -> carros[index].equipa) {
            team = i;
            break;
        }
    }

    while(shm -> carros[index].distPerc < (shm -> distancia_volta*shm -> num_voltas)) {
        sleep(1/shm -> tempo);

        //recebe avaria
        if (msgrcv(msqid, &notificacao, sizeof(notificacao), shm -> carros[index].id+1, IPC_NOWAIT) > 0) {
            
            sprintf(msg, "NEW PROBLEM IN CAR %d", shm -> carros[index].numero);
            
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
            
            sem_wait(mutex_write);
            shm ->carros[index].avaria = true;
            strcpy(shm -> carros[index].estado,"seguranca");
            sem_post(mutex_write);

            write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));

            sprintf(msg, "CAR %d NOW ON SECURITY STATE", shm -> carros[index].numero);

            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
        }

        //estado corrida
        if (strcmp(shm -> carros[index].estado,"corrida")==0) {
            sem_wait(mutex_write);
            shm -> carros[index].distPerc += shm -> carros[index].velocidade*shm -> tempo;
            shm -> carros[index].combustivel -= shm -> carros[index].consumo*shm -> tempo;
            shm -> carros[index].tempo += shm -> tempo;
            sem_post(mutex_write);

            if (shm->terminar == true && (shm -> carros[index].distPerc+1)/shm -> distancia_volta > shm -> carros[index].voltas+1) {
                esperaVolta(index,team);
            }

            //não tem combustivel para 2 voltas
            if (shm->carros[index].combustivel < aux2) {
            sem_wait(mutex_write);
            strcpy(shm -> carros[index].estado,"seguranca"); 
            sem_post(mutex_write);

            write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));

            sprintf(msg, "CAR %d NOW ON SECURITY STATE", shm -> carros[index].numero);

            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
        
            //não tem combustivel para 4 voltas, começa a tentar entrar na box
            } else if (shm->carros[index].combustivel < aux1) {

                //está livre, reserva
                if (strcmp(shm -> equipas[team].estadoBox,"livre") == 0) {
                    sem_wait(mutex_write);
                    strcpy(shm -> equipas[team].estadoBox,"reservada");
                    shm -> equipas[team].reservou = index;
                    sem_post(mutex_write);

                    /*sprintf(msg, "BOX of TEAM %c RESERVED", shm -> equipas[team].nome);

                    sem_wait(mutex_log);
                    escreverLog(msg);
                    sem_post(mutex_log);*/
                }

                //cruza a meta e tenta entrar na box (se não for a última volta)
                if (((shm -> carros[index].distPerc+1)/shm -> distancia_volta > shm -> carros[index].voltas+1) 
                    && shm -> carros[index].voltas+1 < shm -> num_voltas) {
                
                    if (shm->terminar == true) esperaVolta(index,team);

                    //apenas se não estiver carro na box
                    if ((strcmp(shm -> equipas[team].estadoBox,"reservada") == 0) && (shm -> equipas[team].reservou == index)) {

                        sem_wait(mutex_write);
                        shm -> carros[index].distPerc = (shm -> carros[index].voltas+1)*shm -> distancia_volta;
                        sem_post(mutex_write);
                        
                        inBox(index,team,"REFILL");

                        //box fica livre
                        sem_wait(mutex_write);
                        strcpy(shm -> equipas[team].estadoBox,"livre");
                        shm -> equipas[team].reservou = -1;
                        sem_post(mutex_write);

                        /*sprintf(msg, "BOX of TEAM %c FREE", shm -> equipas[team].nome);

                        sem_wait(mutex_log);
                        escreverLog(msg);
                        sem_post(mutex_log);*/

                        //carro entra de novo em modo de corrida
                        sem_wait(mutex_write);
                        strcpy(shm -> carros[index].estado,"corrida"); 
                        sem_post(mutex_write);

                        write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));
                    
                        sprintf(msg, "CAR %d NOW ON RACE STATE", shm -> carros[index].numero);

                        sem_wait(mutex_log);
                        escreverLog(msg);
                        sem_post(mutex_log);
                    }
                }
            }


        //estado seguranca
        } else if (strcmp(shm -> carros[index].estado,"seguranca")==0) {
            
            //está livre, reserva
            if (strcmp(shm -> equipas[team].estadoBox,"livre") == 0) {
                sem_wait(mutex_write);
                strcpy(shm -> equipas[team].estadoBox,"reservada");
                shm -> equipas[team].reservou = index;
                sem_post(mutex_write);

                /*sprintf(msg, "BOX of TEAM %c RESERVED", shm -> equipas[team].nome);

                sem_wait(mutex_log);
                escreverLog(msg);
                sem_post(mutex_log);*/
            }

            sem_wait(mutex_write);
            shm -> carros[index].distPerc += 0.3*shm -> carros[index].velocidade*shm -> tempo;
            shm -> carros[index].combustivel -= 0.4*shm -> carros[index].consumo*shm -> tempo;
            shm -> carros[index].tempo += shm -> tempo;
            sem_post(mutex_write);

            //volta a cruzar a meta e tenta entrar na box (se não for a última volta)
            if (((shm -> carros[index].distPerc+1)/shm -> distancia_volta > shm -> carros[index].voltas+1) 
                && shm -> carros[index].voltas+1 < shm -> num_voltas) {
                
                if (shm->terminar == true) esperaVolta(index,team);

                //apenas se não estiver carro na box
                if ((strcmp(shm -> equipas[team].estadoBox,"reservada") == 0) && (shm -> equipas[team].reservou == index)) {

                    sem_wait(mutex_write);
                    shm -> carros[index].distPerc = (shm -> carros[index].voltas+1)*shm -> distancia_volta;
                    sem_post(mutex_write);
                    
                    //em modo segurança, o carro pode apenas precisar de abastecer, apenas de reparação ou ambos
                    if (shm -> carros[index].avaria == true) inBox(index,team,"REPAIR");
                    else inBox(index,team,"REFILL");

                    //box fica livre
                    sem_wait(mutex_write);
                    strcpy(shm -> equipas[team].estadoBox,"livre");
                    shm -> equipas[team].reservou = -1;
                    sem_post(mutex_write);

                    /*sprintf(msg, "BOX of TEAM %c FREE", shm -> equipas[team].nome);

                    sem_wait(mutex_log);
                    escreverLog(msg);
                    sem_post(mutex_log);*/

                    //carro entra de novo em modo de corrida
                    sem_wait(mutex_write);
                    strcpy(shm -> carros[index].estado,"corrida"); 
                    sem_post(mutex_write);

                    write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));
                
                    sprintf(msg, "CAR %d NOW ON RACE STATE", shm -> carros[index].numero);

                    sem_wait(mutex_log);
                    escreverLog(msg);
                    sem_post(mutex_log);
                }
            }
        }

        //estado desistencia (fica sem combustivel)
        if (shm -> carros[index].combustivel <= 0) {
            sem_wait(mutex_write);
            strcpy(shm -> carros[index].estado,"desistencia"); 
            sem_post(mutex_write);
            

            //printf("%d",fd_pipe[team][1]);
            write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));
            

            sprintf(msg, "CAR %d NOW ON QUIT STATE", shm -> carros[index].numero);
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
            break;
        }

        sem_wait(mutex_write);
        shm -> carros[index].voltas = shm -> carros[index].distPerc/shm -> distancia_volta;
        sem_post(mutex_write);

        //printf("carro: %d | equipa: %c | Dist perc: %f | fuel: %f | voltas: %d | tempo: %d\n",  
        //shm -> carros[index].numero,shm -> carros[index].equipa,shm -> carros[index].distPerc,
        //shm -> carros[index].combustivel, shm -> carros[index].voltas, shm -> carros[index].tempo);
    }

    //carro acabou
    sem_wait(mutex_write);
    if (shm -> carros[index].combustivel > 0) strcpy(shm -> carros[index].estado,"terminado"); 
    shm->carrosPista--;
    shm -> carros[index].distPerc = (shm -> carros[index].voltas)*shm -> distancia_volta;
    sem_post(mutex_write);

    if (shm -> carros[index].combustivel > 0) {
        
        write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));

        sprintf(msg, "CAR %d NOW ON END STATE", shm -> carros[index].numero);
        sem_wait(mutex_log);
        escreverLog(msg);
        sem_post(mutex_log);
    }

    pthread_exit(NULL);
}

void esperaVolta(int index,int team) {
        char msg[SIZE] = "\0";

        sem_wait(mutex_write);
        shm -> carros[index].distPerc = (shm -> carros[index].voltas)*shm -> distancia_volta;
        strcpy(shm -> carros[index].estado,"terminado"); 
        shm->carrosPista--;
        sem_post(mutex_write);

        write(fd_pipe[team][1],&shm -> carros[index].estado,sizeof(shm -> carros[index].estado));

        sprintf(msg, "CAR %d NOW ON END STATE", shm -> carros[index].numero);
                
        sem_wait(mutex_log);
        escreverLog(msg);
        sem_post(mutex_log);
    
        pthread_exit(NULL);
}

void teamManager() { //-------------------------------------------------
    
    int main_pid = getpid();
    int index;
    team equipa;

    //descobre a respetiva equipa na shared memory
    for (int i = 0; i < shm -> countEquipas; i++) {
        if (main_pid == shm -> equipas[i].id) {
            equipa = shm -> equipas[i];
            break;
        }
    }

    //fechar canal de leitura
    //close(fd_pipe[index][0]);

    //criar carros (threads)
    for (int i = 0; i < shm -> countCarros; i++) {
        if (shm -> carros[i].equipa == equipa.nome) {
        sem_wait(mutex_write);
        shm -> id[i] = i;
        shm -> carros[i].idEquipa = main_pid;
        shm -> carros[i].id = i;
        sem_post(mutex_write);

        pthread_create(&my_thread[i], NULL, carro, &shm -> id[i]);
        }
    }

    //waits for threads to die
    for (int i = 0; i < shm -> countCarros; i++) {
        if (shm -> carros[i].equipa == equipa.nome) pthread_join(my_thread[i], NULL);
    }
    exit(0); 
}

int verifOut(int fd) {
    int out;
    char comando[SIZE] = "\0";
    char msg[SIZE] = "\0";
    bool flag = true;
    int nChars;
    
    while (flag) {

        nChars = read(fd, comando, sizeof(comando));
        comando[nChars-1]='\0'; 

        out = verifComando(comando);

        if (out == 0) {
            strcpy(msg,"NEW CAR LOADED => ");
            strcat(msg,substring(comando, 7, 70));
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
            
        } else if (out == -1) {
            strcpy(msg,"WRONG COMMAND => ");
            strcat(msg,comando);
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
        } else if (out == -2) {
            strcpy(msg,"NUMBER OF TEAMS EXCEEDED");
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
    } else if (out == -3) {
            strcpy(msg,"CAR NUMBER ALREADY EXISTS => ");
            strcat(msg,substring(comando, 16, 23));
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
    } else if (out == -4) {
            strcpy(msg,"NUMBER OF CARS EXCEEDED => ");
            strcat(msg,substring(comando, 7, 14));
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
    } else if (out == -5) {
            strcpy(msg,"CANNOT START, NOT ENOUGH TEAMS");
            sem_wait(mutex_log);
            escreverLog(msg);
            sem_post(mutex_log);
    } else if (out == 1) {
            sem_wait(mutex_log);
            escreverLog("RACE IS STARTING!");
            sem_post(mutex_log);
            flag = false;
    }
    }
}

void reset() {
    //corrida ainda não começou (false)
    sem_wait(mutex_write);
    shm->estadoCorrida = false;
    sem_post(mutex_write);

    for (int i = 0; i < shm -> countEquipas; i++) {
        sem_wait(mutex_write);
        strcpy(shm ->equipas[i].estadoBox,"livre");
        shm ->equipas[i].reservou = -1;
        sem_post(mutex_write);
    }

    for (int i = 0; i < shm -> countCarros; i++) {
        sem_wait(mutex_write);
        shm ->carros[i].combustivel = shm -> capacidade;
        shm ->carros[i].distPerc = 0;
        shm ->carros[i].voltas = 0;
        shm ->carros[i].pBox = 0;
        shm ->carros[i].tempo = 0;
        shm ->carros[i].avaria = false;

        shm->carrosPista = 0;
        shm -> totalAvarias = 0;
        shm -> totalAbast = 0;
        sem_post(mutex_write);
    }
}

void raceManager() { //------------------------------------------------------

    char comando[SIZE] = "\0";
    int main_pid = getpid();
    int fd;

    //criar ficheiro bash para kill do -SIGUSR1
    FILE *bash;

    bash = fopen("sigusr1.sh","w");

    fprintf(bash, "#!/bin/bash\n\n");
    fprintf(bash, "kill -SIGUSR1 %d",main_pid);
    fclose(bash);

    reset();

    if ((fd = open(PIPE_NAME, O_RDONLY)) < 0) {
        perror("Cannot open pipe for reading: ");
        exit(0); 
    }

    verifOut(fd);

    sem_post(mutex_start);
    kill(getppid(),SIGUSR2);

    sem_wait(mutex_write);
    shm->terminar = false;
    shm->estadoCorrida = true;
    sem_post(mutex_write);

    //abrir unnamed pipes
    for (int i = 0; i < shm->num_equipas; i++) pipe(fd_pipe[i]);

    //criar processos equipas
    for (int i = 0; i < shm->num_equipas; i++) {
        if (getpid() == main_pid && fork() == 0) {
            sem_wait(mutex_write);
            shm->equipas[i].id = getpid();
            sem_post(mutex_write);
            teamManager();
        }
    }

    if (getpid() == main_pid) {

    //para receber o sinal sigusr1
    signal(SIGUSR1,handle_sigusr1);

    char estado[SIZE];
    int nChars;
    int count = 0;

    // I/O Multiplexing durante a corrida
    while (1) {
        if(count>=shm->countCarros) {
            sem_wait(mutex_write);
            shm->estadoCorrida = false;
            sem_post(mutex_write);
            break;
        }

        estado[0]='\0';

        FD_ZERO(&read_set); 
        
        //set unnamed
        for (int i = 0; i<shm->countEquipas; i++) FD_SET(fd_pipe[i][0],&read_set); 

        //set named
        FD_SET(fd,&read_set);
        
        if(select(fd_pipe[shm->countEquipas-1][0]+1, &read_set,NULL,NULL,NULL) > 0) {
            if(FD_ISSET(fd,&read_set)){
                read(fd, comando, sizeof(comando));
                sem_wait(mutex_log);
                escreverLog("COMMAND REJECTED => RACE ALREADY STARTED!");
                sem_post(mutex_log);
            }

            for (int i = 0; i<shm->countEquipas; i++) {
                if(FD_ISSET(fd_pipe[i][0],&read_set)){
                    nChars=read(fd_pipe[i][0], estado, sizeof(estado));
                    estado[nChars-1]='\0'; 
                    //printf("\n\n%s\n\n",estado);
                    if ((strcmp(estado,"terminado") == 0) || (strcmp(estado,"desistencia") == 0)) {
                        count++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < shm->num_equipas; i++) wait(NULL);

    for (int i=0;i<shm->num_equipas;i++) {
        close(fd_pipe[i][0]); 
        close(fd_pipe[i][1]); 
    }

    kill(shm->malfPid,SIGKILL);

    if (shm->terminar == false) kill(getppid(),SIGTERM);
    exit(0);
    }
}

void malfunctionManager() { //-----------------------------------------------
    int my_pid = getpid();
    msgbuf notificacao;
    int aux = 0;
    int count;

    sem_wait(mutex_write);
    shm->malfPid = my_pid;
    sem_post(mutex_write);

    srand(my_pid);

    strcpy(notificacao.mtext,"avaria");

    while (1) {
        count=0;

    sleep((1/shm->tempo)*shm->T_Avaria);

    for (int i = 0; i<shm->countCarros; i++) {
        if (strcmp(shm->carros[i].estado,"terminado") == 0 || strcmp(shm->carros[i].estado,"desistencia") == 0) count++;
    }

    if (count >= shm->countCarros) exit(0);

    for (int i = 0; i<shm->countCarros; i++) {
        notificacao.mtype = shm->carros[i].id+1;
        aux = rand() % 101;
        if (aux > shm->carros[i].fiabilidade && strcmp(shm->carros[i].estado,"corrida") == 0) {

            sem_wait(mutex_write);
            shm -> totalAvarias++;
            sem_post(mutex_write);

            if ((msgsnd(msqid, &notificacao, sizeof(notificacao),0)) < 0) {
                fprintf(stderr,"msgsnd error: %s\n",strerror(errno));
                exit(1);
            }
        }
    }
    }
    exit(0);
}