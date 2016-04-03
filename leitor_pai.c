#include "leitor_escritor.h"
#include "leitor.h"

/* Declaração de variáveis comuns ao thread pai e seus filhos */
pthread_mutex_t mutex;
sem_t sem_may_send, sem_may_receive;
char buffer[N][FILE_NAME_DIM];
int receive_ptr = 0;
int finish = 0;

/* MAIN */
int main(){

  /* Declaracao de variaveis */
char* a;
  char file_name[FILE_NAME_DIM];
  int n_bytes_read, i, n_bytes, offset, send_ptr=0;
  void *ret_value[N]; 
  pthread_t tid[N];
  
  /* inicialização dos semáforos e do mutex */
  sem_init(&sem_may_send, 0, N);
  sem_init(&sem_may_receive, 0, 0);
  pthread_mutex_init(&mutex, NULL);

  /* Criacao de N threads que executam, cada um, a função reader indefinidamente */
  for(i=0; i<N; i++){
    if (pthread_create(&(tid[i]), NULL, wrapper, NULL) != 0)
      return -1;
  }
  
  /* ciclo de leitura de nomes de ficheiros do stdin e escrita no buffer */
  n_bytes = (FILE_NAME_DIM)*sizeof(char);
  offset = sizeof(char);

  while(1) {

    for(i=0; i<FILE_NAME_DIM;i++){
      file_name[i]='\0';
    }

    if (read(0, file_name, n_bytes) < 0) {
      return -1;
    }

    file_name[12] ='\0';
    printf("Li:%sX\n", file_name);

    if (file_name[0]=='\0') {
      finish = 1;
      for(i=0; i<N; i++) {
        printf("Pode acabar\n");
        sem_post(&sem_may_receive);
        
      }  
      break;
    }

    if (sem_wait(&sem_may_send) == -1){     
      return -1;
    }
    
    strncpy(buffer[send_ptr], file_name, n_bytes + 1);
    send_ptr = (send_ptr+1)%N;

    if (sem_post(&sem_may_receive) == -1) {
      return -1;
    }

  }

/*  O processo-pai fica a espera que os N threads morram e imprime os valores que eles devolvem */
  
  for(i=0; i<N; i++){
    if (pthread_join(tid[i], (void*) &ret_value[i]) != 0)
      return -1;
    //printf("Filho do leitor_pai terminou\n");
    printf("Filho do leitor_pai terminou e devolveu %d\n", (int)ret_value[i]);
  }
  return 0;
}

