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

PONTO clustersArray[K];   // array de centroides: contém as localizações dos centróides tendo o nº do cluster correspondente como indice no array
float ponto_x[N];
float ponto_y[N];


float euclidiana(float ponto_x,float ponto_y, PONTO b){
    float x = ponto_x - (b->x);
    float y = ponto_y - (b->y);
    return x*x + y*y;
}

void inicializa() {
        srand(10);
        
        // a. Iniciar um vetor com valores aleatórios (N amostras no espaço (x,y) )
        #pragma unroll(4)
        for(int i = 0; i < N; i++) {
                ponto_x[i] = (float) rand() / RAND_MAX;
                ponto_y[i] = (float) rand() / RAND_MAX;
        }

        // b. Iniciar os K clusters com as coordenadas das primeiras K amostras
        for(int i = 0; i < K; i++) {
                clustersArray[i] =  (PONTO) malloc(sizeof(struct _ponto));
                clustersArray[i]->x = ponto_x[i];
                clustersArray[i]->y = ponto_y[i];
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
        PONTO p;
        float min_dist;
        int min_indice;

        for(int i=0;i<N;i++) {
        	min_dist=euclidiana(ponto_x[i],ponto_y[i], clustersArray[0]);
        	min_indice=0;
        	for(int j=1;j<K;j++) {
                        dist = euclidiana(ponto_x[i],ponto_y[i], clustersArray[j]);
        		if(dist<min_dist) {
        			min_dist=dist;
        			min_indice=j;
        			}
        		}
                count[min_indice]++;
                sum_x[min_indice] += ponto_x[i];
                sum_y[min_indice] += ponto_y[i];
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
