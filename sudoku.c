#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int sudoku[9][9];
int isCorrect[3] = {0, 0, 0};

typedef struct {
	int column,row;
} box_coordinates;