#include "leitor_escritor.h"

/* MAIN */

int main() {

  char c;
  char string[FILE_NAME_DIM];
  int i, n_bytes, return_escritor, return_leitor, offset;
  int fds[2];
  pid_t pid_escritor, pid_pai, pid_leitor;
  /* Criação do pipe entre o monitor e o leitor_pai */

  if (pipe(fds) != 0) {
    printf("Erro no pipe");
    return -1;
  }

  /* Criação de processo filho que executa o escritor_paralelo */
  if( (pid_escritor = fork()) == 0 ) {
    if (execl("./escritor_paralelo", "escritor_paralelo", NULL) == -1) {
      printf("Erro no exec\n");
      return -1;
    }
  }
  else if (pid_escritor == -1) {
    printf("Falha no fork\n");
    return -1;
  }


  /* Criação de processo filho que executa o leitor_pai */
  if( (pid_leitor = fork()) == 0 ) {                   //codigo do filho
    close(fds[1]);
    dup2(fds[0], 0);
    if (execl("./leitor_pai", "leitor_pai", NULL) == -1) {
      printf("Erro no exec\n");
      return -1;
    }
  }
  else if (pid_leitor == -1) {
    printf("Falha no fork\n");
    return -1;
  }
  else {                                         //codigo do pai
    close(fds[0]);
  }

  offset = sizeof(char);

  n_bytes = FILE_NAME_DIM * sizeof(char);

  while(1) {

    if (read(0, string, n_bytes) < 0) {
      return -1;
    }
    
    for (i=0; i < FILE_NAME_DIM; i++) {
      if (string[i]=='\n'||string[i]==' ') {
        string[i]='\0';
        break;
      }
    }

    if (strcmp(string, "sair") == 0) {
      printf("Recebi sair - monitor\n");
      break;
    }

    else if (strcmp(string, "il") == 0) {
      printf("Enviei il\n");
      kill(pid_escritor, SIGUSR1);
    }

    else if (strcmp(string, "ie") == 0) {
      printf("Enviei ie\n");
      kill(pid_escritor, SIGUSR2);
    }

    else {
      write(fds[1], string, n_bytes);
    }
  }

  close(fds[1]);
  kill(pid_escritor, SIGTSTP);
  if (waitpid(pid_leitor, &return_leitor, 0) == -1) {
    printf("Erro no wait do leitor\n");
    return -1;
  }
  printf("leitor_pai terminou\n");
  if (waitpid(pid_escritor, &return_escritor, 0) == -1) {
    printf("Erro no wait do escritor\n");
    return -1;
  }
  printf("Escritor_paralelo terminou\n");
  
  return 0;
}    


  
