#include <stdio.h>
#include <omp.h>
#define N 1000 // Size of the matrices

int numThreads = 5;
void displayMatrix(int **matrix) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int **A, int **B, int **C) {
    omp_set_num_threads(numThreads);
    #pragma omp parallel for schedule(dynamic)
    for (int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
           for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j]; // Perform matrix multiplication for N x N
            }
        }
    }
}