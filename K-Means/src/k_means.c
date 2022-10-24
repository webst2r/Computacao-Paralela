#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 10000000      // numero de pontos 10000000
#define K 4             // numero de clusters

typedef struct _ponto{
    float x;
    float y;
    int k;    // cluster a que o ponto pertence
} *PONTO;


PONTO pontosArray[N];     // array de pontos
PONTO clustersArray[K];   // array de centroides: contém as localizações dos centróides tendo o nº do cluster correspondente como indice no array


// Distância euclidiana
float euclidiana(PONTO a, PONTO b){
    float x = (a->x) - (b->x);
    float y = (a->y) - (b->y);
    return x*x + y*y;
}

void inicializa() {
        srand(10);
        
        // a. Iniciar um vetor com valores aleatórios (N amostras no espaço (x,y) )
        for(int i = 0; i < N; i++) {
                pontosArray[i] = (PONTO) malloc(sizeof(struct _ponto));
                pontosArray[i]->x = (float) rand() / RAND_MAX;
                pontosArray[i]->y = (float) rand() / RAND_MAX; 
        }

        // b. Iniciar os K clusters com as coordenadas das primeiras K amostras
        for(int i = 0; i < K; i++) {
                clustersArray[i] =  (PONTO) malloc(sizeof(struct _ponto));
                clustersArray[i]->x = pontosArray[i]->x;
                clustersArray[i]->y = pontosArray[i]->y;
        }
}


void atribui_cluster(){

        int count[K]; // em cada indice encontra-se o COUNT de pontos pertencentes a esse cluster
        float sum_x[K];
        float sum_y[K];

        for(int i=0; i < K; i++){
                sum_x[i]=0;
                sum_y[i]=0;
                count[i]=0;
        }

        float dist;
        for(int i=0;i<N;i++) {
        	float min_dist=euclidiana(pontosArray[i], clustersArray[0]);
        	int min_indice=0;
        	for(int j=1;j<K;j++) {
        		dist = euclidiana(pontosArray[i], clustersArray[j]);
        		if(dist<min_dist) {
        			min_dist=dist;
        			min_indice=j;
        			}
        		}
                count[min_indice]++;
                sum_x[min_indice] += pontosArray[i]->x;
                sum_y[min_indice] += pontosArray[i]->y;
        	}

        // Atribuir a cada cluster as suas novas coordenadas e o seu novo size
        for(int i=0; i< K ; i++){
                clustersArray[i]->x = sum_x[i] / count[i];
                clustersArray[i]->y = sum_y[i] / count[i];
                clustersArray[i]->k = count[i];
        }
}

int main() {
        inicializa();
        
        int iterations = 0;
        int count = 0;
        float x[K];
        float y[K];
        while(count != K) {
                
                for(int i=0; i<K; i++){
                        x[i] = clustersArray[i]->x;
                        y[i] = clustersArray[i]->y;
                }
                
                atribui_cluster();
                iterations++;
                count = 0;

                // Verificar se os centroides convergiram (nao mudaram)
                for(int i=0; i<K; i++){
                        if(x[i] == clustersArray[i]->x && y[i] == clustersArray[i]->y){
                                count++;
                        }
                }
        } 
        iterations--;

        printf("\nN = %d, K = %d\n", N, K);
        for(int i=0; i < K; i++){
                printf("Center: (%.3f, %.3f) : Size: %d\n",clustersArray[i]->x,clustersArray[i]->y, clustersArray[i]->k);
        }
        printf("Iterations: %d\n", iterations);
        
        return 0;
}
