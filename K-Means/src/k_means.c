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
int old[N];

// Distância euclidiana
float euclidiana(PONTO a, PONTO b){
    float x = (a->x) - (b->x);
    float y = (a->y) - (b->y);
    float dist = sqrt(pow(x, 2) + pow(y, 2)) * 1.0;
    return dist;
}

void atribui_cluster(){
        float distancias[K];
        int numOfChanges = 0;

        // c. Atribuir cada amostra ao cluster mais próximo usando a distância euclidiana
        for(int i = 0; i < N; i++){
                for(int c = 0; c < K; c++){
                        distancias[c] = euclidiana(pontosArray[i], clustersArray[c]);
                }

                int indice = 0;
                for(int i=1; i < K; i++){
                        if(distancias[i] < distancias[indice]){
                                indice = i;
                                numOfChanges++;
                        }
                }
                pontosArray[i]->k = indice;
        }

        printf("\n changes: %d\n", numOfChanges);
}

void inicializa() {
        srand(10);
        
        // a. Iniciar um vetor com valores aleatórios (N amostras no espaço (x,y) )
        for(int i = 0; i < N; i++) {
                PONTO ponto = (PONTO) malloc(sizeof(struct _ponto));
                ponto->x = (float) rand() / RAND_MAX;
                ponto->y = (float) rand() / RAND_MAX;
                pontosArray[i] = ponto;
        }

        // b. Iniciar os K clusters com as coordenadas das primeiras K amostras
        for(int i = 0; i < K; i++) {
                PONTO cluster = (PONTO) malloc(sizeof(struct _ponto));
                cluster->x = pontosArray[i]->x;
                cluster->y = pontosArray[i]->y;
                clustersArray[i] = cluster;
        }

        // c. Atribuir cada amostra ao cluster mais próximo usando a distância euclidiana
        float distancias[K]; // array para guardar distancias de um ponto aos diferentes centroides, para sacar o minimo deles
        for(int i=0; i<N; i++){
                for(int c=0; c < K; c++){
                        distancias[c] = euclidiana(pontosArray[i], clustersArray[c]);
                }
                int indice = 0;
                for(int i=1; i < K; i++){
                        if(distancias[i] < distancias[indice]){
                                indice = i;
                        }
                }
                pontosArray[i]->k = indice;
        }
}




// Assumindo que o cluster é o mesmo
PONTO soma_pontos(PONTO a, PONTO b){
    PONTO res = malloc(sizeof(struct _ponto));
    res->x = (a->x) + (b->x);
    res->y = (a->y) + (b->y);
    res->k = a->k;
    return res;
}


// Funcao para recalcular os novos centroides para cada cluster
// N é o numero total de data points e K é o numero total de clusters
void calcula_centroides(){
    int count[K]; // em cada indice encontra-se o COUNT de pontos pertencentes a esse cluster
    PONTO sum[K]; 

    for(int i=0; i < K; i++){
        sum[i] = malloc(sizeof(struct _ponto));
    }

    for(int i=0; i< N ; i++){
        count[pontosArray[i]->k]++;
        sum[pontosArray[i]->k] = soma_pontos(pontosArray[i],sum[pontosArray[i]->k] );
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
        
        while(count != N){
                calcula_centroides();

                //int old[N]; // guardar valores antigo para checkar convergencia
                
                for(int i=0; i<N; i++){
                        old[i] = pontosArray[i]->k;
                }
                
                atribui_cluster();
                iterations++;
                count = 0;

                // Verificar se os pontos estão a convergir
                for(int i=0; i<N; i++){
                        if(old[i] == pontosArray[i]->k){
                                count++;
                        }
                }
                printf("IT: [%d]", iterations);
        }


        printf("N = %d, K = %d\n", N, K);
        for(int i=0; i < K; i++){
                printf("Center: (%f, %f) : Size: %d\n",clustersArray[i]->x,clustersArray[i]->y, clustersArray[i]->k);
        }
        printf("Iterations: %d\n", iterations);
        
        return 0;
}
