#include "nqueensmp.h"
#include <omp.h>

int main() {
    int n = 15;
    int numThreads =4;
    omp_set_num_threads(numThreads);
    
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int* board = (int*)malloc(n * sizeof(int));
    
        if (board == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            continue;
        }
        for (int i = 0; i < n; i++) {
            board[i] = -1;
        }
        board[0] = i; //put the first queen in the top row so each itteration is different

        solveNQueensUtil(board, 1, n);

        free(board);
    }

    return 0;
}
