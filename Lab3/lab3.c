#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

//gcc lab3.c -o lab3 -Wall -lpthread

float *vetor;
float menorSeq;
float maiorSeq;

void imprimeVetor(int  dim, float *vetor) {
        for (int i = 0; i < dim; i++)
            printf("%.5f ", vetor[i]);
        printf ("\n");
}

float gerador(float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

//fluxo principal
int main(int argc, char* argv[]) {
    int dim;
    if(argc<2) {
      printf("Digite: %s <tamanho do vetor>\n", argv[0]);
      return 1;
    }
    dim = atoi(argv[1]);
   
    vetor = (float *) malloc(sizeof(float) * dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}

    srand(time(0));
    for(int i=0; i<dim; i++)
      vetor[i] = gerador(0, 100);
    
    for(int i = 0; i < dim; i++){
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

    imprimeVetor(dim, vetor);

    printf("Menor numero: %.5f\n", menorSeq);
    printf("Maior numero: %.5f\n", maiorSeq);

    return 0;
}