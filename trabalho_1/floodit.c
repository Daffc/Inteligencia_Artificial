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
    printf("\n\n");
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
                Painting
========================================*/  

void paint(TGame * game, char prev_color, char next_color, int row, int col){
    
    if((row < 0) || (col < 0) || (row >= game->rows) || (col >= game->cols))
        return;
    if((game->board[row * game->cols + col] != prev_color))
        return;

    game->board[row * game->cols + col] = next_color;

    paint(game, prev_color, next_color, row, col++);
    paint(game, prev_color, next_color, row, col--);
    paint(game, prev_color, next_color, row++, col);
    paint(game, prev_color, next_color, row--, col);
}

/*========================================
                MAIN
========================================*/  
int main(){
    TGame game;

    initiateGame(&game);
    
    printGame(&game);

    paint(&game, game.board[0], 3, 0, 0);
    printBoard(&game);

    paint(&game, game.board[0], 2, 0, 0);
    printBoard(&game);

    paint(&game, game.board[0], 1, 0, 0);
    printBoard(&game);

    paint(&game, game.board[0], 3, 0, 0);
    printBoard(&game);

    paint(&game, game.board[0], 2, 0, 0);
    printBoard(&game);

    return 0;
}