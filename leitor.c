#include "leitor_escritor.h"

/* READER */

int reader () {

  /* Declaracao de variaveis */
 
  char text_compare_1[STRING_DIM], text_compare_2[STRING_DIM], file_name[FILE_NAME_DIM];
  int file, i, lock;
  size_t n_bytes;
  int n_bytes_buffer;
int inteiro;
  char strings[N_STRING][STRING_DIM] = {"aaaaaaaaa\n","bbbbbbbbb\n","ccccccccc\n","ddddddddd\n","eeeeeeeee\n","fffffffff\n","ggggggggg\n","hhhhhhhhh\n","iiiiiiiii\n","jjjjjjjjj\n"};

  /* cálculo do numero de bytes a ler */

  n_bytes_buffer = FILE_NAME_DIM*sizeof(char);
  n_bytes = STRING_DIM*sizeof(char);
  /* ciclo de leituras */

  while(1) {

    /* verificação se há nomes de ficheiros para receber */

    if (sem_wait(&sem_may_receive) == -1) {
      printf("Erro no semáforo wait\n");
      return -1;
    }

    if (finish) {
      return 0;
    }

    /* secção crítica onde o nome do ficheiro é lido */

    if (pthread_mutex_lock(&mutex)!=0) {
      printf("Erro no mutex_lock");
      return -1;
    }

    strncpy(file_name, buffer[receive_ptr], n_bytes_buffer);
    receive_ptr = (receive_ptr+1) % N;

    if ((pthread_mutex_unlock(&mutex)) == -1) {
      printf("Erro no mutex_unlock\n");
      return -1;
    }

    if (sem_post(&sem_may_send)==-1) {
      printf("Erro no sem_post\n");
      return -1;
    }

    /* Abertura do Ficheiro */
 
    file = open(file_name, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (file==-1) {
      printf("Erro na abertura do ficheiro\n");
      return -1;
    }

    /* So pode ler se nenhum escritor estiver a escrever nesse ficheiro */

    if((lock = flock(file, LOCK_SH)) != 0) {
      printf("Erro no lock\n"); 
      return -1;
    }

    /* Leitura do Ficheiro */    

    if (n_bytes != read(file, text_compare_1, n_bytes)) {
      lock = flock(file, LOCK_UN);
      close(file);
      printf("O ficheiro %s contém erros\n", file_name);
      return -1;
    }
    for (i=1; i<LOOP_2; i++){
      if (n_bytes != read(file, text_compare_2, n_bytes)) {
        lock=flock(file,LOCK_UN);
        close(file);
        printf("Erro na leitura do ficheiro (system call: read)\n", file_name);
        return -1;
      }
      if (strncmp(text_compare_1, text_compare_2, n_bytes) != 0){
        lock=flock(file, LOCK_UN);
        close(file);
        printf("O ficheiro %s contém erros\n", file_name);
        return -1;
      }
    }

    /* Apos escrever permite que outro escritor escreva no ficheiro */

    if ((lock = flock(file, LOCK_UN)) != 0) {
      printf("Erro no unlock");
      return -1;
    }

    /* Fecho do Ficheiro */

    if (close(file) != 0) {
      printf("Erro no fecho do ficheiro");
      return -1;
    }

    printf("O ficheiro %s não contém erros\n", file_name);
    
   // if (finish) {
    //  break;
    //}
  }
  //printf("Acabei: sou leitor filho\n");
  //return 0;
}



/* WRAPPER */

void* wrapper(void* nothing){
  return (void*) reader();
}
