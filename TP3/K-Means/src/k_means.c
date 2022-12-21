#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int N, K, N_PROCESSES; // numero de pontos, clusters, fios de execução
#define MAX 20

float euclidiana(float ponto1_x,float ponto1_y, float ponto2_x,float ponto2_y){
    float x = ponto1_x - ponto2_x;
    float y = ponto1_y - ponto2_y;
    return x*x + y*y;
}

void inicializa(float ponto_x[], float ponto_y[], float clusters_x[], float clusters_y[]) {
        srand(10);
        
        // a. Iniciar um vetor com valores aleatórios (N amostras no espaço (x,y) )
        for(int i = 0; i < N; i++) {
                ponto_x[i] = (float) rand() / RAND_MAX;
                ponto_y[i] = (float) rand() / RAND_MAX;
        }

        // b. Iniciar os K clusters com as coordenadas das primeiras K amostras
        for(int i = 0; i < K; i++) {
                clusters_x[i] = ponto_x[i];
                clusters_y[i] = ponto_y[i];
        }
}


int main(int argc, char *argv[]) {
        if(argc==4) {
                N = atoi(argv[1]);
                K = atoi(argv[2]);
                N_PROCESSES = atoi(argv[3]);
                int rank,size;
                MPI_Init(NULL, NULL);
                MPI_Comm_rank(MPI_COMM_WORLD, &rank);
                MPI_Comm_size(MPI_COMM_WORLD, &size);

                float *clusters_x,*clusters_y,*ponto_x, *ponto_y, *recv_x, *recv_y,*sum_x,*sum_y,*total_sum_x,*total_sum_y;
                clusters_x = malloc(sizeof(float)*K);
                clusters_y = malloc(sizeof(float)*K);
                ponto_x = malloc(sizeof(float) * N);
                ponto_y = malloc(sizeof(float) * N);
                recv_x = malloc(sizeof(float) * ((N/N_PROCESSES) + 1));
                recv_y = malloc(sizeof(float) * ((N/N_PROCESSES) + 1));
                sum_x = malloc(sizeof(float)*K);
                sum_y = malloc(sizeof(float)*K);
                total_sum_x = malloc(sizeof(float)*K);
                total_sum_y = malloc(sizeof(float)*K);
                int *clusters_size,*count,*total_count;
                clusters_size = malloc(sizeof(int)*K);
                count = malloc(sizeof(int)*K);
                total_count = malloc(sizeof(int)*K);

                if(rank==0) {//Master
                        inicializa(ponto_x, ponto_y, clusters_x,clusters_y);
                }

                //particionamento dos pontos
                MPI_Scatter(ponto_x, (N/N_PROCESSES) + 1, MPI_FLOAT,
		recv_x, (N/N_PROCESSES) + 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

	        MPI_Scatter(ponto_y, (N/N_PROCESSES) + 1, MPI_FLOAT,
		recv_y, (N/N_PROCESSES) + 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

                int iterations = 0;
                int changed = 0;
                float dist;
                float min_dist;
                int min_indice;
                int point_process= N/N_PROCESSES;
                
                while(iterations<MAX && changed == 0) {
                        
                        MPI_Bcast(clusters_x, K, MPI_FLOAT, 0, MPI_COMM_WORLD);
		        MPI_Bcast(clusters_y, K, MPI_FLOAT, 0, MPI_COMM_WORLD);


                        for(int i=0; i < K; i++){
                                sum_x[i]=0;
                                sum_y[i]=0;
                                count[i]=0;
                        }

                        for(int i=0;i<point_process;i++) {
                                min_dist=euclidiana(recv_x[i],recv_y[i], clusters_x[0],clusters_y[0]);
                                min_indice=0;
                                for(int j=1;j<K;j++) {
                                        dist = euclidiana(recv_x[i],recv_y[i], clusters_x[j],clusters_y[j]);
                                        if(dist<min_dist) {
                                                min_dist=dist;
                                                min_indice=j;
                                                }
                                        }
                                count[min_indice]++;
                                sum_x[min_indice] += recv_x[i];
                                sum_y[min_indice] += recv_y[i];
                        }

                        MPI_Reduce(sum_x, total_sum_x, K, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
                        MPI_Reduce(sum_y, total_sum_y, K, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
                        MPI_Reduce(count, total_count, K, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

                        
                        if (rank==0) {
                                iterations++;
                                int number_clusters_change = 0;
                                // Verificar se os centroides convergiram (nao mudaram)
                                for(int i=0; i<K; i++){
                                        if(clusters_x[i] ==total_sum_x[i]/total_count[i]  && clusters_x[i] == total_sum_y[i]/total_count[i]){
                                                number_clusters_change++;
                                        }
                                }
                                if(number_clusters_change==K) {
                                        changed=1;
                                }                                 
                                for (int i=0; i<K; i++) {
                                        clusters_x[i] = total_sum_x[i]/total_count[i];
                                        clusters_y[i] = total_sum_y[i]/total_count[i];
                                        clusters_size[i] = total_count[i];
                                }
                        }
                        MPI_Bcast(&changed, 1, MPI_INT, 0, MPI_COMM_WORLD);
                        MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);
                }


                if(rank == 0) {
                        printf("\nN = %d, K = %d\n", N, K);
                        for(int i=0; i < K; i++){
                                printf("Center: (%.3f, %.3f) : Size: %d\n",clusters_x[i],clusters_y[i], clusters_size[i]);
                        }
                        printf("Iterations: %d\n", iterations);
                }
                MPI_Finalize();
                return 0;
        }
        else {
                printf("Erro: Argumentos inválidos.\n");
        }
}