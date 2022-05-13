#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include "timer.h"

//gcc lab4.c -o lab4 -Wall -lpthread -lm

long int dim;
long int *vetor;
float *vetorSaidaSequencial;
float *vetorSaidaConcorrente;
long int nthreads;
pthread_mutex_t mutex;
long int indice = 0;

void imprimeVetorInt(long int  dim, long int *vetor) {
        for (long int i = 0; i < dim; i++)
            printf("%ld ", vetor[i]);
        printf ("\n");
}

void imprimeVetorFloat(long int  dim, float *vetor) {
        for (long int i = 0; i < dim; i++)
            printf("%.2f ", vetor[i]);
        printf ("\n");
}

long int gerador(long int min, long int max )
{
    return rand() % (max + min - min) + min;      /* [min, max] */
}

long int ehPrimo(long int n) {
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (long int i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;
}

void processaPrimos(long int *vetorEntrada, float *vetorSaida, long int dim) {
    for(int i=0; i<dim; i++) {
        if (ehPrimo(vetorEntrada[i]))
            vetorSaida[i] = sqrt(vetorEntrada[i]);
        else
            vetorSaida[i] = vetorEntrada[i];
    }
}

void *ExecutaTarefa (void *threadid) {
    while(indice < dim){
     pthread_mutex_lock(&mutex);
     long int i = indice++;
     pthread_mutex_unlock(&mutex);
     vetorSaidaConcorrente[i] = ehPrimo(vetor[i]) ? sqrt(vetor[i]) : vetor[i];
    }
  pthread_exit(NULL);
}

void verificador(float *vetorSeq, float *vetorConc, int dim){
    for (int i = 0; i < dim; i++)
        if(vetorSeq[i] != vetorConc[i]){
            printf("Os vetores não batem\n");
            break;
        }
}

int main(int argc, char* argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    double inicio, fim, deltaSeq, deltaConc, aceleracao;    
    if(argc<3) {
      printf("Digite: %s <tamanho do vetor> <numero de threads>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    vetor = (long int *) malloc(sizeof(long int) * dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}
    vetorSaidaSequencial = (float *) malloc(sizeof(float) * dim);
    if (vetorSaidaSequencial == NULL) {printf("ERRO--malloc\n"); return 2;}
    vetorSaidaConcorrente = (float *) malloc(sizeof(float) * dim);
    if (vetorSaidaConcorrente == NULL) {printf("ERRO--malloc\n"); return 2;}

    srand(time(0));
    for(long int i=0; i<dim; i++){
      vetor[i] = gerador(1, 100000000);
      vetorSaidaSequencial[i] = 0;
      vetorSaidaConcorrente[i] = 0;
    }
    GET_TIME(inicio);
    processaPrimos(vetor, vetorSaidaSequencial, dim);
    GET_TIME(fim);
    deltaSeq = fim - inicio;
    printf("Tempo Sequencial:%lf\n", deltaSeq);

    GET_TIME(inicio);
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    //--inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);

    //criacao das threads
    for(int t=0; t<nthreads; t++) {
        if (pthread_create(tid+t, NULL, ExecutaTarefa, NULL)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }
    //--espera todas as threads terminarem
    for (int t=0; t<nthreads; t++) {
        if (pthread_join(*(tid+t), NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 
    pthread_mutex_destroy(&mutex);

    GET_TIME(fim);
    deltaConc = fim - inicio;
    printf("Tempo Concorrente:%lf\n", deltaConc);

    aceleracao = deltaSeq/deltaConc;
    printf("Aceleracao: %lf\n", aceleracao);

    verificador(vetorSaidaSequencial, vetorSaidaConcorrente, dim);

    //imprimeVetorInt(dim, vetor);
    //imprimeVetorFloat(dim, vetorSaidaSequencial);
    //imprimeVetorFloat(dim, vetorSaidaConcorrente);
    free(vetor); 
    free(vetorSaidaSequencial);
    free(vetorSaidaConcorrente);
    free(tid);       
    return 0;
}