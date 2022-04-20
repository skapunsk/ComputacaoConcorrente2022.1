#include<stdio.h>
#include<pthread.h>

#define NTHREADS 2
#define NELEMENTOS 10

//gcc lab1.c -o lab1 -Wall -lpthread

int vetor[NELEMENTOS];

//funcao que a thread ira executar
void * tarefa (void * arg) {
   int ident = * (int *) arg;
   for(int i = ident; i <= NELEMENTOS; i+=NTHREADS){
        vetor[i-1] = i * i;
        printf("Ola, sou o número %d!\n", vetor[i-1]);
   }
   pthread_exit(NULL);
}

//funcao principal
int main(void) {
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS]; //identificador local da thread
    //cria as threads 
    for(int i=1; i<=NTHREADS; i++) {
       ident[i-1] = i;
       if (pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1])) 
          printf("ERRO -- pthread_create\n");
    }
    //espera as threads terminarem 
    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
    //imprimir o vetor
    for(int i=0; i<NELEMENTOS; i++) 
       printf("Numero = %d Quadrado = %d\n", i+1, vetor[i]);

    //Verificador
    for(int i = 1; i <= NELEMENTOS; i++){
        //printf("Numero = %d Posicao = %d\n", vetor[i-1], i);
        if(vetor[i-1] != i*i){
            printf("Ha um valor errado!\n");
            break;
        }
    }

    //desvincula o termino da main do termino programa
    //pthread_exit(NULL);
    return 0;
}
