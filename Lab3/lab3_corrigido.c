#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

//gcc lab3.c -o lab3 -Wall -lpthread
long int dim;
float *vetor;
float menorSeq;
float maiorSeq;
float menorConc;
float maiorConc;
long int nthreads;


void imprimeVetor(long int  dim, float *vetor) {
        for (long int i = 0; i < dim; i++)
            printf("%.5f ", vetor[i]);
        printf ("\n");
}

float gerador(float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

typedef struct{
   long int id;
   float menorLocal; //identificador do elemento que a thread ira processar
   float maiorLocal; //dimensao das estruturas de entrada
} tArgs;

//funcao que as threads executarao
void *procura(void *arg) {
   tArgs *args = (tArgs*) arg;
   long int id = (long int) args->id;
   long int tam_bloco = dim/nthreads;
   long int ini = id*tam_bloco;
   long int fim = ini+tam_bloco;
   /*float *menorLocalThread;
   menorLocalThread = (float*) malloc(sizeof(float));
   float *maiorLocalThread;
   maiorLocalThread = (float*) malloc(sizeof(float)); 
   *menorLocalThread = (float)args->menorLocal;
   *maiorLocalThread = (float)args->maiorLocal;*/
   if (id==nthreads-1) fim=dim; //trata sobra
   else fim = ini + tam_bloco;
   for(long int i=ini; i<fim; i++){
       if(vetor[i] < args->menorLocal){
            args->menorLocal = vetor[i];
       }
       if(vetor[i] > args->maiorLocal){
            args->maiorLocal = vetor[i];
       }
   }
   //args->menorLocal = *menorLocalThread;
   //args->maiorLocal = *maiorLocalThread;
   pthread_exit((void *) args);
}

//fluxo principal
int main(int argc, char* argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao    
    double inicio, fim, deltaSeq, deltaConc, aceleracao;
    tArgs *retorno;

    if(argc<3) {
      printf("Digite: %s <tamanho do vetor> <numero de threads>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;
   
    vetor = (float *) malloc(sizeof(float) * dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}

    srand(time(0));
    for(long int i=0; i<dim; i++)
      vetor[i] = gerador(0, 100);
    
    //imprimeVetor(dim, vetor);

    GET_TIME(inicio);
    for(long int i = 0; i < dim; i++){
        if(i == 0){
            menorSeq = vetor[i];
            maiorSeq = vetor[i];
        }else{
            if(vetor[i] < menorSeq)
                menorSeq = vetor[i];
            if(vetor[i] > maiorSeq)
                maiorSeq = vetor[i];
        }
    }
    GET_TIME(fim);
    deltaSeq = fim - inicio;
    printf("Tempo Sequencial:%lf\n", deltaSeq);

    //*menorConc = vetor[0];
    //*maiorConc = vetor[0];

    GET_TIME(inicio);
    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}
    //criacao das threads
    for(long int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->menorLocal = vetor[i];
        (args+i)->maiorLocal = vetor[i];
        if(pthread_create(tid+i, NULL, procura, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 

    //espera pelo termino da threads
    for(long int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), (void**)&retorno);
        if(i == 0){
            menorConc = retorno->menorLocal;
            maiorConc = retorno->maiorLocal;
        }
        if(menorConc > retorno->menorLocal)
            menorConc = retorno->menorLocal;
        if(maiorConc < retorno->maiorLocal)
            maiorConc = retorno->maiorLocal;
    }
    GET_TIME(fim);   
    deltaConc = fim - inicio;
    printf("Tempo Concorrente: %lf\n", deltaConc);
    aceleracao = deltaSeq/deltaConc;
    printf("Aceleracao: %lf\n", aceleracao);

    printf("Menor numero sequencial: %.5f\n", menorSeq);
    printf("Maior numero sequencial: %.5f\n", maiorSeq);

    printf("Menor numero concorrente: %.5f\n", menorConc);
    printf("Maior numero concorrente: %.5f\n", maiorConc);

    return 0;
}
