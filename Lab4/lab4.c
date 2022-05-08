#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include "timer.h"

//gcc lab4.c -o lab4 -Wall -lpthread -lm

long int dim;
long int *vetor;
float *vetorSaidaSequencial;

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

int main(int argc, char* argv[]) {
    double inicio, fim, deltaSeq, aceleracao;    
    if(argc<2) {
      printf("Digite: %s <tamanho do vetor>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
    vetor = (long int *) malloc(sizeof(long int) * dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}
    vetorSaidaSequencial = (float *) malloc(sizeof(float) * dim);
    if (vetorSaidaSequencial == NULL) {printf("ERRO--malloc\n"); return 2;}

    srand(time(0));
    for(long int i=0; i<dim; i++){
      vetor[i] = gerador(1, 100);
      vetorSaidaSequencial[i] = 0;
    }
    GET_TIME(inicio);
    processaPrimos(vetor, vetorSaidaSequencial, dim);
    GET_TIME(fim);
    deltaSeq = fim - inicio;
    printf("Tempo Sequencial:%lf\n", deltaSeq);

    //imprimeVetorInt(dim, vetor);
    //imprimeVetorFloat(dim, vetorSaidaSequencial);
    free(vetor); 
    free(vetorSaidaSequencial);       
    return 0;
}
