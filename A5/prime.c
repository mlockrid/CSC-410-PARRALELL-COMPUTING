#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

void sieveOfEratosthenes(int n, int rank, int size) {
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    for (int i = 0; i <= n; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    //set starts and ends
    int num = n / size;
    int start = rank*num;
    int end = start+num;

    if(rank==size-1) {
        //last row, change end to n
        end = n;
    }

    for (int p = 2; p <= sqrt(n); p++) {
        MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    printf("Prime numbers up to %d are:\n", n);
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    free(is_prime);
}

int main(int argc, char** argv) {
    // Dynamically allocate memory for the matrices
    double start_time = MPI_Wtime();
    MPI_Init(&argc, &argv);
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 10000000;
    sieveOfEratosthenes(n);
    double end_time = MPI_Wtime();
    if (rank == 0) {
        printf("Total execution time: %f seconds\n", end_time - start_time);
    }
    MPI_Finalize();
    return 0;
}
