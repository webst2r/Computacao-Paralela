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
    float dist = sqrt(pow(x, 2) + pow(y, 2)) * 1.0;
    return dist;
}

void atribui_cluster(){
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
        	pontosArray[i]->k = min_indice;
        	}
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

        // c. Atribuir cada amostra ao cluster mais próximo usando a distância euclidiana       
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
        	pontosArray[i]->k = min_indice;
        	}
}


// Funcao para recalcular os novos centroides para cada cluster
// N é o numero total de data points e K é o numero total de clusters
void calcula_centroides(){
    int count[K]; // em cada indice encontra-se o COUNT de pontos pertencentes a esse cluster
    PONTO sum[K]; 

    for(int i=0; i < K; i++){
        sum[i] = malloc(sizeof(struct _ponto));
        count[i] = 0;
    }

    for(int i=0; i< N ; i++){ //soma pontos
        count[pontosArray[i]->k]++;
        sum[pontosArray[i]->k]->x += pontosArray[i]->x;
        sum[pontosArray[i]->k]->y += pontosArray[i]->y;
        sum[pontosArray[i]->k]->k = pontosArray[i]->k;
    }
    
    // Atribuir a cada cluster as suas novas coordenadas e o seu novo size
    for(int i=0; i< K ; i++){
        clustersArray[i]->x = (sum[i]->x) / count[i];
        clustersArray[i]->y = (sum[i]->y) / count[i];
        clustersArray[i]->k = count[i];
    }
    
}

int main() {
        inicializa();
        
        int iterations = 0;
        int count = 0;

        PONTO old[K]; // guardar valores antigos dos centroides para checkar convergencia
        for(int i=0; i<K; i++){
                old[i] = malloc(sizeof(struct _ponto));
        }
        
        while(count != K) {
                
                for(int i=0; i<K; i++){
                        old[i]->x = clustersArray[i]->x;
                        old[i]->y = clustersArray[i]->y;
                        old[i]->k = clustersArray[i]->k;
                }
                calcula_centroides();
                
                atribui_cluster();
                iterations++;
                count = 0;

                // Verificar se os centroides convergiram (nao mudaram)
                for(int i=0; i<K; i++){
                        if(old[i]->x == clustersArray[i]->x && old[i]->y == clustersArray[i]->y && old[i]->k == clustersArray[i]->k){
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
