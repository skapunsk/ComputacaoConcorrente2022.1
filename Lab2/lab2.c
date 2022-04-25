#include<stdio.h>
#include<stdlib.h>

int *mat1; //matriz 1
int *mat2; //matriz 2
int *matResSeq; //matriz resultante sequencial

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

int main(int argc, char* argv[]) {
    int dim; //dimensao das matrizes

    //leitura e avaliacao dos parametros de entrada
    if(argc<2) {
        printf("Digite: %s <dimensao da matriz>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);

    //alocacao de memoria para as estruturas de dados
    mat1 = (int *) malloc(sizeof(int) * dim * dim);
    if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
    mat2 = (int *) malloc(sizeof(int) * dim * dim);
    if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
    matResSeq = (int *) malloc(sizeof(int) * dim * dim);
    if (matResSeq == NULL) {printf("ERRO--malloc\n"); return 2;}

    //inicializacao das estruturas de dados de entrada e saida
    for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat1[i*dim+j] = rand() % (100 + 1 - 1) + 1;
         mat2[i*dim+j] = rand() % (100 + 1 - 1) + 1;
         matResSeq[i*dim+j] = 0;    //equivalente mat[i][j]
      }
    }

    multiplicaoSequencial(mat1, mat2, matResSeq, dim);

    puts("Matriz 1");
    imprimeMatriz(dim, mat1);
    puts("\nMatriz 2");
    imprimeMatriz(dim, mat2);
    puts("\nMatriz Resultante");
    imprimeMatriz(dim, matResSeq);


    free(mat1);
    free(mat2);
    free(matResSeq);

    return 0;
}