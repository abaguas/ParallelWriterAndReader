# ifndef __LEITOR_ESCRITOR_H_
# define __LEITOR_ESCRITOR_H_

/* DIRETIVAS DE PRE-COMPLIADOR */

#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include "leitor.h"
#include "escritor.h"
#include "escritor_paralelo.h"

#define N 3
#define N_FILES 5
#define FILE_NAME_DIM 13
#define N_STRING 10
#define STRING_DIM 10
#define LOOP_1 512
#define LOOP_2 1024
#define WRITE_PROCESSES 2
#define READ_PROCESSES 3

/* VARIAVEIS COMUNS AOS PROGRAMAS */

extern int finish;
extern int over;
extern int irregular_write;
extern int exclusive_lock;
extern pthread_mutex_t mutex;
extern sem_t sem_may_send, sem_may_receive;
extern char buffer[N][FILE_NAME_DIM];
extern int receive_ptr;
# endif

