#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct t_Game{
    char *board;
    int rows;
    int cols;
    int colors;
} TGame;


/*========================================
            DEBUG PRINTS
========================================*/

// Print state of board.
void printBoard(TGame *game){
       for(int i = 0; i < game->rows; i++){
        for(int j = 0; j < game->cols; j++){
            printf("%d ", game->board[i * game->cols + j]);
        }
        printf("\n");
    }
}

// Print state of Game.
void printGame(TGame *game){

    printf("row: %d\ncols: %d\ncolors: %d\n\n", game->rows, game->cols, game->colors);

    printBoard(game);
}

/*========================================
            GAME FUNCTIONS
========================================*/  

// Print state of board.
void initializeBoard(TGame *game){
    char *line = NULL, *s_num;
    long unsigned int limit = 100;

    for(int i = 0; i < game->rows; i++){
        getline(&line, &limit, stdin);
        s_num = strtok(line," ");
        for(int j = 0; j < game->cols; j++){
            game->board[i * game->cols + j] = atoi(s_num);
            s_num = strtok(NULL," ");
        }
    }
}

// Initiating Game dimensions and board.
void initiateGame(TGame *game){
    char *line = NULL, *s_num;
    long unsigned int limit = 100;

    // Get first line and store dimensions and colors.
    getline(&line, &limit, stdin);
    s_num = strtok(line," ");
    game->rows = atoi(s_num); 
    s_num = strtok(NULL," ");
    game->cols = atoi(s_num); 
    s_num = strtok(NULL," ");
    game->colors = atoi(s_num); 

    // Alocate space for board.
    game->board = (char *) malloc( game->rows * game->cols * sizeof(char));

    // Initialize board values.
    initializeBoard(game);
}


/*========================================
                MAIN
========================================*/  
int main(){
    TGame game;

    initiateGame(&game);
    
    printGame(&game);

    return 0;
}