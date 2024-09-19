#include <stdio.h>

#define N 10 // Size of the matrices


void displayMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            C[i][j] = A[i][j]*B[i][j];
        }
    }
}