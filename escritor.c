# include "leitor_escritor.h"
 
/* WRITER */

int writer(){

  /* Declaracao de variaveis */
  
  int file, random, close_success, lock, counter;
  size_t n_bytes;
  char files[N_FILES][FILE_NAME_DIM] = {"SO2014-0.txt","SO2014-1.txt","SO2014-2.txt","SO2014-3.txt","SO2014-4.txt"};
  char strings[N_STRING][STRING_DIM] = {"aaaaaaaaa\n","bbbbbbbbb\n","ccccccccc\n","ddddddddd\n","eeeeeeeee\n","fffffffff\n","ggggggggg\n","hhhhhhhhh\n","iiiiiiiii\n","jjjjjjjjj\n"};
  char alternative_strings[N_STRING][STRING_DIM] = {"xaaaaaaaa\n","bbbbbbbbx\n","ccccccxcc\n","ddxdddddd\n","eeeeeexee\n","fffffxfff\n","ggggggxgg\n","hhhhxhhhh\n","iiiixiiii\n","jjjjjxjjj\n"};

  /* Ciclo de Abertura, Escrita de cadeia aleatoria e Fecho de Ficheiros */  

  while(1) {
    counter = 0;                                      //inicialização e atualização do valor do contador

    /* Abertura do Ficheiro */

    srand((unsigned) time(NULL));
    random = rand()%(N_FILES);
    file = open(files[random], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
    if (file == -1) {
      return -1;
    }

    /* Escreve no ficheiro caso ninguem esteja a escrever nele */
    if (exclusive_lock) {
      if( (lock = flock(file,LOCK_EX) ) != 0){
        printf("Erro no lock\n");
        return -1;
      }
    }

    random = rand()%(N_STRING);
    n_bytes = (STRING_DIM)*sizeof(char);

    /* Escrita do ficheiro */

    while (counter < LOOP_2){
      if (write(file, strings[random], n_bytes) == -1) {
        printf("Erro no write\n");  
        if (exclusive_lock) {
          lock = flock(file, LOCK_UN);
        }
        close(file);
        return -1;
      }
      counter ++;
      if (irregular_write) {
        if (write(file, alternative_strings[random], n_bytes) == -1) {
          if (exclusive_lock) {
            lock = flock(file, LOCK_UN);
          }
          close(file);
          return -1;
        }
        counter ++;
      }
    }

    /* Apos escrever permite que outro escritor escreva no ficheiro */
    if (exclusive_lock) {
      if((lock=flock(file, LOCK_UN))!=0) {
        return -1;
      }
    }

    /* Fecho do ficheiro */
    close_success = close(file);
    if(close_success!=0) {
      return -1;
    }

    if (over) {
      return 0;
    }
  }

  return 0;
}

    
/* WRAPPER */

void* wrapper2(void* nothing){
  return (void*) writer();
}


