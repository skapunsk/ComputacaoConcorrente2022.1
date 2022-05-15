#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//gcc lab5.c -o lab5 -Wall -lpthread

#define NTHREADS  5

int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, x_cond_fim;

/* Thread A */
void *A (void *t) {
  int boba1, boba2;
  
  /* faz alguma coisa pra gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("Seja bem-vindo!\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_cond_broadcast(&x_cond);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {

  pthread_mutex_lock(&x_mutex);
  if (x == 0) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  if(x > 3)
       pthread_cond_signal(&x_cond_fim);
  printf("Fique a vontade.\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {

  pthread_mutex_lock(&x_mutex);
  if (x == 0) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  if(x > 3)
       pthread_cond_signal(&x_cond_fim);
  printf("Sente-se por favor.\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {

  pthread_mutex_lock(&x_mutex);
  if (x == 0) { 
     pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  if(x > 3)
       pthread_cond_signal(&x_cond_fim);
  printf("Aceita um copo d’agua?.\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread E */
void *E (void *t) {

  pthread_mutex_lock(&x_mutex);
  if(x < 4)
  pthread_cond_wait(&x_cond_fim, &x_mutex);
  printf("Volte sempre!.\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);
  pthread_cond_init (&x_cond_fim, NULL);

  /* Cria as threads */
  pthread_create(&threads[4], NULL, A, NULL);
  pthread_create(&threads[3], NULL, B, NULL);
  pthread_create(&threads[2], NULL, C, NULL);
  pthread_create(&threads[1], NULL, D, NULL);
  pthread_create(&threads[0], NULL, E, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  pthread_cond_destroy(&x_cond_fim);
}