#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 9

int sudoku[N][N]={};

typedef struct {
	int column,row;
    int (* sudoku)[9];
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
    

    parameters * param0 = (parameters *) malloc(sizeof(parameters));
    param0->row = 0;
    param0->column = 0;
    param0->sudoku = sudoku;
    
    // ====== Create the parameters for the 3x3 threads ======
    
    // First 3x3
    parameters * param1 = (parameters *) malloc(sizeof(parameters));
    param1->row = 0;
    param1->column = 0;
    param1->sudoku = sudoku;
    
    // Second 3x3
    parameters * param2 = (parameters *) malloc(sizeof(parameters));
    param2->row = 0;
    param2->column = 3;
    param2->sudoku = sudoku;
    
    // Third 3x3
    parameters * param3 = (parameters *) malloc(sizeof(parameters));
    param3->row = 0;
    param3->column = 6;
    param3->sudoku = sudoku;
    
    // Fourth 3x3
    parameters * param4 = (parameters *) malloc(sizeof(parameters));
    param4->row = 3;
    param4->column = 0;
    param4->sudoku = sudoku;
    
    // Fifth 3x3
    parameters * param5 = (parameters *) malloc(sizeof(parameters));
    param5->row = 3;
    param5->column = 3;
    param5->sudoku = sudoku;
    
    // Sixth 3x3
    parameters * param6 = (parameters *) malloc(sizeof(parameters));
    param6->row = 3;
    param6->column = 6;
    param6->sudoku = sudoku;
    
    // Seventh 3x3
    parameters * param7 = (parameters *) malloc(sizeof(parameters));
    param7->row = 6;
    param7->column = 0;
    param7->sudoku = sudoku;
    
    // Eighth 3x3
    parameters * param8 = (parameters *) malloc(sizeof(parameters));
    param8->row = 6;
    param8->column = 3;
    param8->sudoku = sudoku;
    
    // Ninth 3x3
    parameters * param9 = (parameters *) malloc(sizeof(parameters));
    param9->row = 6;
    param9->column = 6;
    param9->sudoku = sudoku;
    
    // ====== Create the threads ======
    pthread_t thread_rows, thread_cols, thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9;
    
    // ====== Create the return values for the threads ======
    void * all_rows;
    void * all_cols;
    void * square1;
    void * square2;
    void * square3;
    void * square4;
    void * square5;
    void * square6;
    void * square7;
    void * square8;
    void * square9;
    
    // ====== Initialize the threads ======
    pthread_create(&thread_rows, NULL, walkRows, (void *) param0);
    pthread_create(&thread_cols, NULL, walkColumns, (void *) param0);
    pthread_create(&thread1, NULL, checkSquare, (void *) param1);
    pthread_create(&thread2, NULL, checkSquare, (void *) param2);
    pthread_create(&thread3, NULL, checkSquare, (void *) param3);
    pthread_create(&thread4, NULL, checkSquare, (void *) param4);
    pthread_create(&thread5, NULL, checkSquare, (void *) param5);
    pthread_create(&thread6, NULL, checkSquare, (void *) param6);
    pthread_create(&thread7, NULL, checkSquare, (void *) param7);
    pthread_create(&thread8, NULL, checkSquare, (void *) param8);
    pthread_create(&thread9, NULL, checkSquare, (void *) param9);

    // ======= Wait for all threads to finish their tasks =======
    pthread_join(thread_rows, &all_rows);
    pthread_join(thread_cols, &all_cols);
    pthread_join(thread1, &square1);
    pthread_join(thread2, &square2);
    pthread_join(thread3, &square3);
    pthread_join(thread4, &square4);
    pthread_join(thread5, &square5);
    pthread_join(thread6, &square6);
    pthread_join(thread7, &square7);
    pthread_join(thread8, &square8);
    pthread_join(thread9, &square9);
    

    for (int x = 0; x < N; x++){
                for (int y = 0; y < N; y++) {
                    fprintf(solution, "%d ", sudoku[x][y]);
                    if (y == 8) {
                        fprintf(solution, "\n");
                    }
                }
        }
    // ====== Check whether the Sudoku Puzzle was solved ======
    if (    (int) all_rows == 1 &&
            (int) all_cols == 1 &&
            (int) square1 == 1 &&
            (int) square2 == 1 &&
            (int) square3 == 1 &&
            (int) square4 == 1 &&
            (int) square5 == 1 &&
            (int) square6 == 1 &&
            (int) square7 == 1 &&
            (int) square8 == 1 &&
            (int) square9 == 1 ) {
                
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