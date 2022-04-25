/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int *mat1; //matriz 1
int *mat2; //matriz 2
int *matResSeq; //matriz resultante sequencial
int *matResConc; //matriz resultante concorrente
int nthreads; //numero de threads

void imprimeMatriz(int  dim, int *mat) {
    for (int l = 0;l < dim; l++){
        for (int j = 0; j < dim; j++)
            printf("\t %d", mat[(l*dim)+j]);
        printf ("\n");
    }    
}

void multiplicaoSequencial(int *mat1, int *mat2, int *matResSeq, int dim){
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            for (int k = 0; k < dim; k++)
                matResSeq[(i*dim)+j]= matResSeq[(i*dim)+j] + mat1[(i*dim)+k] * mat2[(k*dim)+j];              
}

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

//funcao que as threads executarao
void *multiplicaoConcorrente(void *arg) {
   tArgs *args = (tArgs*) arg;
   //printf("Thread %d\n", args->id);
   for (int i = args->id; i < args->dim; i+=nthreads)
        for (int j = 0; j < args->dim; j++)
            for (int k = 0; k < args->dim; k++){
                matResConc[(i*(args->dim))+j]= matResConc[(i*(args->dim))+j] + mat1[(i*(args->dim))+k] * mat2[(k*(args->dim))+j];
            }
   pthread_exit(NULL);
}

void verificador(int *matResSeq, int *matResConc, int dim){
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            if(matResSeq[(i*dim)+j] != matResConc[(i*dim)+j]){
                printf("As matrizes não batem\n");
                break;
            }
}

int main(int argc, char* argv[]) {
    int dim; //dimensao das matrizes
    pthread_t *tid; //identificadores das threads no sistema    
    tArgs *args; //identificadores locais das threads e dimensao
    double inicio, fim, deltaSeq, deltaConc, aceleracao;

    //leitura e avaliacao dos parametros de entrada
    if(argc<3) {
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    //alocacao de memoria para as estruturas de dados
    mat1 = (int *) malloc(sizeof(int) * dim * dim);
    if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
    mat2 = (int *) malloc(sizeof(int) * dim * dim);
    if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
    matResSeq = (int *) malloc(sizeof(int) * dim * dim);
    if (matResSeq == NULL) {printf("ERRO--malloc\n"); return 2;}
    matResConc = (int *) malloc(sizeof(int) * dim * dim);
    if (matResConc == NULL) {printf("ERRO--malloc\n"); return 2;}

    //inicializacao das estruturas de dados de entrada e saida
    for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat1[i*dim+j] = rand() % (100 + 1 - 1) + 1;
         mat2[i*dim+j] = rand() % (100 + 1 - 1) + 1;
         matResSeq[i*dim+j] = 0;    //equivalente mat[i][j]
         matResConc[i*dim+j] = 0;
      }
    }
    
    GET_TIME(inicio);
    multiplicaoSequencial(mat1, mat2, matResSeq, dim);
    GET_TIME(fim);
    deltaSeq = fim - inicio;
    printf("Tempo Multiplicacao Sequencial:%lf\n", deltaSeq);
    //alocacao das estruturas
    GET_TIME(inicio);
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 2;}
    //criacao das threads
    for(int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->dim = dim;
        if(pthread_create(tid+i, NULL, multiplicaoConcorrente, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 

    //espera pelo termino da threads
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim);   
    deltaConc = fim - inicio;
    printf("Tempo Multiplicacao com Threads: %lf\n", deltaConc);
    aceleracao = deltaSeq/deltaConc;
    printf("Aceleracao: %lf\n", aceleracao);
    verificador(matResSeq, matResConc, dim);
    /*puts("Matriz 1");
    imprimeMatriz(dim, mat1);
    puts("\nMatriz 2");
    imprimeMatriz(dim, mat2);
    puts("\nMatriz Resultante Sequencial");
    imprimeMatriz(dim, matResSeq);
    puts("\nMatriz Concorrente");
    imprimeMatriz(dim, matResConc);*/

    free(mat1);
    free(mat2);
    free(matResSeq);
    free(matResConc);
    free(args);
    free(tid);

    return 0;
}