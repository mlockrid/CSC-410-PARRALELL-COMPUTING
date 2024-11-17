#include <stdio.h>
#include <stdlib.h> 
#include <mpi.h>

#define N 1000

void displayMatrix(int** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int** A, int** B, int** C, int n, int start,int end) {
    for (int i = start; i < end; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char** argv) {
    // Dynamically allocate memory for the matrices
    double start_time = MPI_Wtime();
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_rows;
    int start_row;
    int end_row;
    num_rows = N/size;
    if (N % size == 0){ //cases where n is divisible by num threads
        start_row = rank*num_rows;
        end_row = start_row+num_rows;
    } else { //cases where n is not divisible     
        if (rank == size-1) { //check for last thread to get rest of rows
            start_row = rank*num_rows;
            end_row = N;
        } else {
            start_row = rank*num_rows;
            end_row = start_row+num_rows; 
        }
    }
    

    int** A = (int**)malloc(N * sizeof(int*));
    int** B = (int**)malloc(N * sizeof(int*));
    int** C = (int**)malloc(N * sizeof(int*));
    
    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }


    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    printf("Matrices allocated successfully.\n");

    // Initialize matrices A and B on starting process
    if (rank ==0){
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                A[i][j] = 1;
                B[i][j] = 1;
                C[i][j] = 0;
            }
        }
    }

    //send matrix to each process
    for (int i = 0; i < N; ++i) {
        MPI_Bcast(B[i], N, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(A[i], N, MPI_INT, 0, MPI_COMM_WORLD);
    }

    printf("Matrices initialized successfully.\n");

    matrixMultiply(A, B, C, N,start_row,end_row);

    printf("Process %d Matrix multiplication complete!\n",rank);

    //have original process gather results from each other process and put it in C
    if (rank==0) {
        int receive_start;
        int receive_end;
        //calculate parts of C that need to be recieved, recycling the old code for start and end above
        for (int i=1; i<size;i++) {
            if (N % size == 0){ //cases where n is divisible by num threads
                receive_start = i*num_rows;
                receive_end = receive_start+num_rows;
            } else { //cases where n is not divisible     
                if (i == size-1) { //check for last thread to get rest of rows
                    receive_start = i*num_rows;
                    receive_end = N;
                } else {
                    receive_start = i*num_rows;
                    receive_end = receive_start+num_rows;
                }
            }
            for (int j = receive_start; j<receive_end; j++) {
                MPI_Recv(C[j],N,MPI_INT,i,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        //this is the send side, each process needs to send it's portion of C to process 0
        for (int i = start_row; i < end_row; i++) {
            MPI_Send(C[i], N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }


    //Optionally display the resulting matrix C
    //if (rank==0){
    //    printf("Resulting Matrix C:\n");
    //    displayMatrix(C, N);
    //}

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    printf("TIME IS: %f seconds\n",elapsed_time);
    MPI_Finalize();
    return 0;
}
