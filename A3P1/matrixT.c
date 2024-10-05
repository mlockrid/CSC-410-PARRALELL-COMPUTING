#include "matrixT.h"

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    // Allocate memory for matrices
    A = (int**)malloc(N * sizeof(int*));
    B = (int**)malloc(N * sizeof(int*));
    C = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Initialize matrices A and B with values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    // Create threads to perform matrix multiplication
    int rows_per_thread = N / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        if (N % NUM_THREADS == 0){ //cases where n is divisible by num threads
            thread_data[i].thread_id = i;
            thread_data[i].num_rows = rows_per_thread;
            thread_data[i].start_row = i*rows_per_thread;
        } else { //cases where n is not divisible
            if (i == NUM_THREADS-1) { //check for last thread to get rest of rows
                thread_data[i].thread_id = i;
                thread_data[i].num_rows = N - (i*rows_per_thread);
                thread_data[i].start_row = i*rows_per_thread;
            } else { //same logic as base case here
                thread_data[i].thread_id = i;
                thread_data[i].num_rows = rows_per_thread;
                thread_data[i].start_row = i*rows_per_thread;
            }
        }       

        /** create threads with entry function matrixMultiplyThread 
            and pass thread_data[i] as an argument **/
             pthread_create(&threads[i], NULL, matrixMultiplyThread, (void*)&thread_data[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Matrix multiplication complete!\n");

    // Display the resulting matrix C
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}