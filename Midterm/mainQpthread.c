#include "nqueenspthread.h"
#include <pthread.h>

int main() {
    int n = 15;
    int numThreads = 4;

    pthread_t threads[numThreads];
    boardData boardData[numThreads];   

    //to divide up the work, we get the integer division result to be the cols per thread, and then the remainder is evenly divided
    //until it runs out
    int colsPerThread = n / numThreads; 
    int extraCols = n % numThreads;

    int currentCol = 0;

    for (int i = 0; i < numThreads; i++) {
        int startCol = currentCol;
        int endCol = startCol + colsPerThread - 1;

        if (extraCols > 0) {
            endCol++;
            extraCols--;
        }
        
        //set starting and ending col for each thread to solve
        boardData[i].startCol = startCol;
        boardData[i].endCol = endCol;
        boardData[i].n = n;
        pthread_create(&threads[i], NULL, solveNQueensThread, &boardData[i]);

        currentCol = endCol + 1;
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
