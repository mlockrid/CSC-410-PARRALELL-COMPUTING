#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000  // Size of the matrix
#define NUM_THREADS 1  // Number of threads

int **A, **B, **C;  // Global matrices

// Information holder for each thread
typedef struct {
    int thread_id;
    int num_rows;
    int start_row;  
} thread_data_t;

// Entry function for each thread
void* matrixMultiplyThread(void* arg) {
    // Extract thread info from the passes argument
    thread_data_t *data = (thread_data_t*) arg; //cast null ptr to thread_data type
    int thread_id = data->thread_id; //derreference the thread id
    int rows_per_thread = data->num_rows; //derreference the num_rows to loop over

    // Calculate the start and ending row chunck for each thread to handle
    int start_row = data->start_row; //same as task 1
    int end_row = start_row + rows_per_thread;

    // Loop through the start and end row assigned to the thread and compute matrix multiplication
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j]; // Perform matrix multiplication for N x N
            }
        }
    }

    pthread_exit(NULL);
}
