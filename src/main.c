/*Nome: Fabrício de Souza Maruta  RA:138313
Laboratorio 5 - EA876
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_WORKERS 4
#define ATIVO 1
#define INATIVO 0


typedef struct {
  unsigned long int N;
  int ID;
} thread_args;

pthread_mutex_t trava;
int n_workers = 0;
pthread_t workers[MAX_WORKERS];
int worker_status[MAX_WORKERS];
int counter = 0;

/*Retorna 1 se é primo e 0 cc*/
int primo(unsigned long int numero){
  if(numero < 2)
    return 0;
  if(numero == 2){
    return 1;
  }
  for(unsigned long int i = 2; i < numero; i++){
    if(numero%i == 0){
      return 0;
    }
  }
  return 1;
}


void* worker(void *arg) {
  thread_args *info = (thread_args *)arg;
  pthread_mutex_lock(&trava);
  int M = primo(info->N);
  if(M == 1){
    counter ++;
  }
  n_workers -= 1;
  worker_status[info->ID] = INATIVO;
  free(info);
  pthread_mutex_unlock(&trava);
  return NULL;
}

int main(int argc, char **argv) {
  int numero_recebido;
  thread_args *send_args;
  int j;

  while (1) {
    if (numero_recebido<0) break;
    if (n_workers >= MAX_WORKERS) {
    } else {
      if(scanf("%d", &numero_recebido) == EOF){
        break;
      }
      pthread_mutex_lock(&trava);

      send_args = (thread_args*)malloc(sizeof(thread_args));
      send_args->N = numero_recebido;
      /* Procura espaco para thread livre */
      j = 0;
      while (worker_status[j] == ATIVO) j++;
      send_args->ID = j;
      worker_status[j] = ATIVO;
      n_workers += 1;
      pthread_create(& (workers[j]), NULL, worker, (void*) send_args);
      pthread_mutex_unlock(&trava);
    }
  }

  /* Esperando threads */
  for (int i=0; i<MAX_WORKERS; i++) {
    if (worker_status[i]==ATIVO) {
      pthread_join(workers[i], NULL);
    }
  }
  printf("%d\n", counter);
  return 0;
}
