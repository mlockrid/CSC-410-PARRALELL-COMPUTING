#include <stdio.h>
#include <mpi.h>

#define N 1000000000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule(int rank, int size) {
    double h = 1.0 / N; 
    double sum = 0; 

    //divide work and force last thread to end on N 
    int num = N/size;
    int start = rank*num;
    int end = start+num;
    if (rank == size-1){
        end = N;
    }

    for (int i = start; i < end; ++i) {
        sum += f(i * h);
    }
    sum *= h; 
    printf("Rank %d sum %f\n",rank,sum);
    return sum;
}

int main(int argc, char** argv) {
    int rank, size;
    double total_sum = 0;
    double process_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);   
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   

    double start_time = MPI_Wtime(); 

    process_sum = trapezoidalRule(rank,size);
    MPI_Reduce(&process_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    //printing time and result on process 0 only
    if (rank == 0) {
        printf("Estimated value of π: %f\n", total_sum);
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        printf("Total execution time: %f seconds\n", elapsed_time);
    }

    MPI_Finalize();
    return 0;
}
