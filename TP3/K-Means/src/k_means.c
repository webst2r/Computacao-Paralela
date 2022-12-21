#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int N, K, N_PROCESSES; // numero de pontos, clusters, fios de execução

typedef struct _ponto{
    float x;
    float y;
    int k;    // cluster a que o ponto pertence
} *PONTO;


float euclidiana(float ponto_x,float ponto_y, PONTO b){
    float x = ponto_x - (b->x);
    float y = ponto_y - (b->y);
    return x*x + y*y;
}

void inicializa(float ponto_x[], float ponto_y[], PONTO clustersArray[]) {
        srand(10);
        
        // a. Iniciar um vetor com valores aleatórios (N amostras no espaço (x,y) )
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


int main(int argc, char *argv[]) {
        if(argc==4) {
                N = atoi(argv[1]);
                K = atoi(argv[2]);
                //N_PROCESSES = atoi(argv[3]);
                int rank,size;
                MPI_Init(NULL, NULL);
                MPI_Comm_rank(MPI_COMM_WORLD, &rank );
                MPI_Comm_size(MPI_COMM_WORLD, &size);

                PONTO clustersArray[K];   // array de centroides: contém as localizações dos centróides tendo o nº do cluster correspondente como indice no array
                float *ponto_x, *ponto_y, *recv_x, *recv_y;
                int i =10;
                if(rank==0) {//Master
                        ponto_x = malloc(sizeof(float) * N);
                        ponto_y = malloc(sizeof(float) * N);
                        inicializa(ponto_x, ponto_y, clustersArray);
                        recv_x = (float *)malloc(sizeof(float) * ((N/N_PROCESSES) + 1));
                        recv_y = (float *)malloc(sizeof(float) * ((N/N_PROCESSES) + 1));
                }//workes
                else {
                        recv_x = (double *)malloc(sizeof(double) * ((N/N_PROCESSES) + 1));
		        recv_y = (double *)malloc(sizeof(double) * ((N/N_PROCESSES) + 1));
                }

                MPI_Scatter(ponto_x, (N/N_PROCESSES) + 1, MPI_FLOAT,
		recv_x, (N/N_PROCESSES) + 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

	        MPI_Scatter(ponto_y, (N/N_PROCESSES) + 1, MPI_FLOAT,
		recv_y, (N/N_PROCESSES) + 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

                int iterations = 0;
                int count = 0;
                float x[K];
                float y[K];
                
                while(iterations<20) {
                        for(int i=0; i<K; i++){
                                x[i] = clustersArray[i]->x;
                                y[i] = clustersArray[i]->y;
                        }
                        
                        atribui_cluster(ponto_x, ponto_y, clustersArray);
                        iterations++;
                        count = 0;
                        if (rank==0) {
                        // Verificar se os centroides convergiram (nao mudaram)
                                for(int i=0; i<K; i++){
                                        if(x[i] == clustersArray[i]->x && y[i] == clustersArray[i]->y){
                                                count++;
                                        }
                                }
                        }
                } //https://github.com/dzdao/k-means-clustering-mpi/blob/master/k-means.c

                if(world_rank == 0) {
                        printf("\nN = %d, K = %d\n", N, K);
                        for(int i=0; i < K; i++){
                                printf("Center: (%.3f, %.3f) : Size: %d\n",clustersArray[i]->x,clustersArray[i]->y, clustersArray[i]->k);
                        }
                        printf("Iterations: %d\n", iterations);
                }
                MPI_Finalize();
        }
        else {
                printf("Erro: Argumentos inválidos.\n");
        }
        return 0;
}