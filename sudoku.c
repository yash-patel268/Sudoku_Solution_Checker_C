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

void readPuzzle(FILE *file){
	for (int x = 0; x < N; x++){
		for (int y = 0; y < N; y++){
			fscanf(file, "%d", &sudoku[x][y]);
		}
	}

}

void * walkRows(void * params){
    parameters * data = (parameters *) params;

    int startRow = data->row;
    int startCol = data->column;

    for (int x = startRow; x < N; ++x){
        int row[10] = {0};

        for (int y = startCol; y < N; ++y){
            int val = data->sudoku[x][y];

            if (row[val] != 0){
                return (void *) 0;
            } else{
                row[val] = 1;
            }
        }
    }

    return (void *) 1;
}

void * walkColumns(void * params){
    parameters * data = (parameters *) params;

    int startRow = data->row;
    int startCol = data->column;

    for (int x = startCol; x < N; ++x){
        int col[10] = {0};

        for (int y = startRow; y < N; ++y){
            int val = data->sudoku[y][x];

            if (col[val] != 0) {
                return (void *) 0;
            } else{
                col[val] = 1;
            }
        }
    }
    return (void *) 1;
}

void * checkSquare(void * params) {
    parameters * data = (parameters *) params;

    int startRow = data->row;
    int startCol = data->column;
    int saved[10] = {0};

    for (int x = startRow; x < startRow + 3; ++x) {
        for (int y = startCol; y < startCol + 3; ++y) {
            int val = data->sudoku[x][y];

            if (saved[val] != 0) {
                return (void *) 0;
            } else{
                saved[val] = 1;
            }
        }
    }
    return (void *) 1;
}

int main(){
    FILE *puzzle = fopen("puzzle.txt", "r");
	readPuzzle(puzzle);
    FILE *solution = fopen("solution.txt", "w");
    

    parameters * initial = (parameters *) malloc(sizeof(parameters));
    initial->row = 0;
    initial->column = 0;
    initial->sudoku = sudoku;
    
    // ====== Create the parameters for the 3x3 threads ======
    
    parameters * param[9];
    // First 3x3
    param[0] = (parameters *) malloc(sizeof(parameters));
    param[0]->row = 0;
    param[0]->column = 0;
    param[0]->sudoku = sudoku;
    
    // Second 3x3
    param[1] = (parameters *) malloc(sizeof(parameters));
    param[1]->row = 0;
    param[1]->column = 3;
    param[1]->sudoku = sudoku;
    
    // Third 3x3
    param[2] = (parameters *) malloc(sizeof(parameters));
    param[2]->row = 0;
    param[2]->column = 6;
    param[2]->sudoku = sudoku;
    
    // Fourth 3x3
    param[3] = (parameters *) malloc(sizeof(parameters));
    param[3]->row = 3;
    param[3]->column = 0;
    param[3]->sudoku = sudoku;
    
    // Fifth 3x3
    param[4] = (parameters *) malloc(sizeof(parameters));
    param[4]->row = 3;
    param[4]->column = 3;
    param[4]->sudoku = sudoku;
    
    // Sixth 3x3
    param[5] = (parameters *) malloc(sizeof(parameters));
    param[5]->row = 3;
    param[5]->column = 6;
    param[5]->sudoku = sudoku;
    
    // Seventh 3x3
    param[6] = (parameters *) malloc(sizeof(parameters));
    param[6]->row = 6;
    param[6]->column = 0;
    param[6]->sudoku = sudoku;
    
    // Eighth 3x3
    param[7] = (parameters *) malloc(sizeof(parameters));
    param[7]->row = 6;
    param[7]->column = 3;
    param[7]->sudoku = sudoku;
    
    // Ninth 3x3
    param[8] = (parameters *) malloc(sizeof(parameters));
    param[8]->row = 6;
    param[8]->column = 6;
    param[8]->sudoku = sudoku;

    pthread_t threads[9];
    pthread_t threadRows, threadColumns;
    void * allRows;
    void * allColumns;
    void * returnValues[9];


    pthread_create(&threadRows, NULL, walkRows, (void *) initial);
    pthread_create(&threadColumns, NULL, walkColumns, (void *) initial);

    for(int x=0; x<9; x++){
        pthread_create(&threads[x], NULL, checkSquare, (void *) param[x]);
    }

    pthread_join(threadRows, &allRows);
    pthread_join(threadColumns, &allColumns);

    for(int x=0; x<9; x++){
        pthread_join(threads[x], &returnValues[x]);
    }

    if (    (int) allRows == 1 &&
            (int) allColumns == 1 &&
            (int) returnValues[0] == 1 &&
            (int) returnValues[1] == 1 &&
            (int) returnValues[2] == 1 &&
            (int) returnValues[3] == 1 &&
            (int) returnValues[4] == 1 &&
            (int) returnValues[5] == 1 &&
            (int) returnValues[6] == 1 &&
            (int) returnValues[7] == 1 &&
            (int) returnValues[8] == 1 ) {
        printf("The Sudoku Puzzle is solved!\n");

        for (int x = 0; x < N; x++){
            for (int y = 0; y < N; y++) {
                fprintf(solution, "%d ", sudoku[x][y]);
                if (y == 8) {
                    fprintf(solution, "\n");
                }
            }
	    }
    }
    else {
        printf("The Sudoku Puzzle is NOT solved.\n");
    }
    
    return 0;
}