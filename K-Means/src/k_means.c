#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _ponto{
    float x;
    float y;
} *PONTO;

#define N 10000000      // numero de pontos
#define K 4             // numero de clusters



void inicializa() {
        PONTO pontosArray[N];
        PONTO clustersArray[K];
        srand(10);
        
        for(int i = 0; i < N; i++) {
                PONTO ponto = (PONTO) malloc(sizeof(struct _ponto));
                ponto->x = (float) rand() / RAND_MAX;
                ponto->y = (float) rand() / RAND_MAX;
                pontosArray[i] = ponto;
        }

        for(int i = 0; i < K; i++) {
                PONTO cluster = (PONTO) malloc(sizeof(struct _ponto));
                cluster->x = pontosArray[i]->x;
                cluster->y = pontosArray[i]->y;
                clustersArray[i] = cluster;
        }
}


int main() {
        printf("Hello World!\n");
        inicializa();
        return 0;
}
