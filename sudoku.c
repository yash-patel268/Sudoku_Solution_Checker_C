#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 9

int sudoku[N][N];
int isCorrect[3] = {0, 0, 0};

typedef struct {
	int column,row;
    int (* sudoku)[9][9];
} parameters;

void readPuzzle(FILE *file) {
	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			fscanf(file, "%d", &sudoku[x][y]);
		}
	}

}

void * walkRows(void * params) {
    parameters * data = (parameters *) params;

    int startRow = data->row;
    int startCol = data->column;

    for (int i = startRow; i < N; ++i) {
        int row[10] = {0};

        for (int j = startCol; j < N; ++j) {
            int val = data->sudoku[i][j];

            if (row[val] != 0) {
                return (void *) 0;
            } else{
                row[val] = 1;
            }
        }
    }
    
    return (void *) 1;
}