
#include "GameFunctions.h"
#include "Graph.h"
#include "stdio.h"

/*========================================
            DEBUG PRINTS
========================================*/

// Print state of board in colors perspective.
void printBoardColors(TGame *game){
    printf("printBoardColors\n");
    for(int i = 0; i < game->rows; i++){
        for(int j = 0; j < game->cols; j++){
            printf("%d ", game->board[i * game->cols + j].color);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Print state of board in node perspective.
void printBoardNode(TGame *game){
    printf("printBoardNode\n");
    for(int i = 0; i < game->rows; i++){
        for(int j = 0; j < game->cols; j++){
            printf("%d ", game->board[i * game->cols + j].node);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Print state of Game.
void printGame(TGame *game){

    printf("row: %d\ncols: %d\ncolors: %d\n\n", game->rows, game->cols, game->colors);
    printBoardColors(game);
}

/*========================================
            GAME FUNCTIONS
========================================*/  

// Print state of board.
void initializeBoard(TGame *game){
    char *line, *s_num;
    long unsigned int limit = 100;

    line = (char *) malloc(100);

    for(int i = 0; i < game->rows; i++){
        getline(&line, &limit, stdin);
        s_num = strtok(line," ");
        for(int j = 0; j < game->cols; j++){
            game->board[i * game->cols + j].color = atoi(s_num);
            s_num = strtok(NULL," ");
        }
    }
    free(line);
}

// Initiating Game dimensions and board.
void initiateGame(TGame *game){
    char *line, *s_num;
    long unsigned int limit = 100;

    line = (char *) malloc(100);

    // Get first line and store dimensions and colors.
    getline(&line, &limit, stdin);
    s_num = strtok(line," ");
    game->rows = atoi(s_num); 
    s_num = strtok(NULL," ");
    game->cols = atoi(s_num); 
    s_num = strtok(NULL," ");
    game->colors = atoi(s_num); 
    
    // Alocate space for board.
    game->board = (struct t_Cell *) malloc( game->rows * game->cols * sizeof(TCell));

    // Signing all cell in board not bellonging to any node.
    for(int i=0; i < game->rows * game->cols; i++){
        game->board[i].node = -1; 
    }

    // Initialize board values.
    initializeBoard(game);
    free(line);
}

// Free Game structures.
void freeGame(TGame *game){
    free(game->board);
}
