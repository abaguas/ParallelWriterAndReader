# include "leitor_escritor.h"
# include "escritor.h"

/* Declaração de variáveis comuns aos threads filhos */

int exclusive_lock = 1, irregular_write = 0, over = 0;

/* MAIN */

int main(){

  /* Declaracao de variaveis */
  struct sigaction new_action_SIGUSR1;
  struct sigaction new_action_SIGUSR2;
  struct sigaction new_action_SIGTSTP;
  struct timeval tvstart;
  struct timeval tvend;
  struct timeval tvduration;
  unsigned int duration;
  pthread_t tid[WRITE_PROCESSES];
  void *ret_value[WRITE_PROCESSES];
  int i,status;
  pid_t pid;

  /* Le a data atual */

  gettimeofday(&tvstart,NULL);

  /* atribuição de rotinas aos signals */

  //signal(SIGUSR1, lock_invertion);
  new_action_SIGUSR1.sa_handler = lock_invertion;
  sigemptyset(&new_action_SIGUSR1.sa_mask);
  sigaddset(&new_action_SIGUSR1.sa_mask, SIGUSR1);
  new_action_SIGUSR1.sa_flags = 0;
  if (sigaction(SIGUSR1, &new_action_SIGUSR1, NULL) == -1) {;
    return -1;
  }

  //signal(SIGUSR2, write_invertion);
  new_action_SIGUSR2.sa_handler = write_invertion;
  sigemptyset(&new_action_SIGUSR2.sa_mask);
  sigaddset(&new_action_SIGUSR2.sa_mask, SIGUSR2);
  new_action_SIGUSR2.sa_flags = 0;
  if (sigaction(SIGUSR2, &new_action_SIGUSR2, NULL) == -1) {
    return -1;
  }

  //signal(SIGTSTP, end_writer);
  new_action_SIGTSTP.sa_handler = end_writer;
  sigemptyset(&new_action_SIGTSTP.sa_mask);
  sigaddset(&new_action_SIGTSTP.sa_mask, SIGTSTP);
  new_action_SIGTSTP.sa_flags = 0;
  if (sigaction(SIGTSTP, &new_action_SIGTSTP, NULL) == -1) {
    return -1;
  }
  
  /* Criacao de WRITE_PROCESSES threads que executam, cada um, o escritor */

  for(i=0; i<WRITE_PROCESSES ; i++){
    if (pthread_create(&(tid[i]), NULL, wrapper2, NULL)!=0)
      return -1;
  }

  /* O processo-pai fica a espera que os WRITE_PROCESSES threads morram */
  
  for(i=0; i<WRITE_PROCESSES; i++){
    if (pthread_join(tid[i], (void*) &ret_value[i]) != 0) {
      return -1;
    }
    printf("Escritor filho terminou com %d\n", (int)ret_value[i]);
  }      

  /* Le a data atual, calcula e imprime a diferenca de datas */

  gettimeofday(&tvend,NULL);

  tvduration.tv_sec = tvend.tv_sec -tvstart.tv_sec;
  tvduration.tv_usec = tvend.tv_usec -tvstart.tv_usec;
  duration = tvduration.tv_sec * 1000000 + tvduration.tv_usec;
  printf("Execution Time: %d microseconds\n", duration);

  return 0;
}


/* LOCK_INVERTION */

void lock_invertion(int sig) {
  printf("signal: recebi lock_invertion\n");
  if (exclusive_lock == 0) {
    exclusive_lock = 1;
  }
  else {
    exclusive_lock = 0;
  }
  
}

/* WRITE_INVERTION */

void write_invertion(int sig) {
  printf("signal: recebi write_invertion\n");
  if (irregular_write == 0) {
    irregular_write = 1;
  }
  else {
    irregular_write = 0;
  }
  
}

/* END_WRITER */

void end_writer(int sig) {
  printf("signal: Recebi sair\n");
  over = 1;
}

