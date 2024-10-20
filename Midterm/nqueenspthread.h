#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    int startCol;
    int endCol;
    int n;
} boardData;

bool isSafe(int board[], int row, int col, int n) {
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

void printBoard(int board[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[j] == i) {
                printf("Q");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void solveNQueensUtil(int board[], int col, int n) {
    // Base case
        // write your code here
    if (col >=n)
    {
        //print out current solution
        //printf("printing solution\n");
        //printBoard(board,n);
        return; 
    }
    // Recursive case
        // write your code here
        //try each column, when safe column is found then call fucntion again to go one row deeper, base case will prevent us from going over.
    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[col] = i; // put queen because board is safe 

            // Recursively place the rest of the queens, only one queen can be put per row
            solveNQueensUtil(board, col + 1, n);
        }
    }
}

void* solveNQueensThread(void* argStruct) {
    boardData* data = (boardData*)argStruct;
    int startCol = data->startCol;
    int endCol = data->endCol;
    int n = data->n;
    for (int i = startCol; i <= endCol; i++) {
        //move board creation into each thread, because threads have their own heap memory, no need to worry about shared memory errors
        int* board = (int*)malloc(n * sizeof(int));
        if (board == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
        }

        for (int i = 0; i < n; i++) {
            board[i] = -1;
        }
        board[0]=i; //put the queen in the first col needs to be here so that way each thread is forced to do a different solution
        solveNQueensUtil(board, 1, n);

        free(board);
        }

}