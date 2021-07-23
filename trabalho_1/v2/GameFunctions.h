#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

#ifndef GAME_FUNCTIONS
#define GAME_FUNCTIONS

    typedef struct t_Cell{
        char color;
        int vertex;
    }TCell;

    typedef struct t_Game{
        struct t_Cell *board;
        int rows;
        int cols;
        int colors;       
    } TGame;

    // Print state of board in colors perspective.
    void printBoardColors(TGame *game);

    // Print state of board in vertex perspective.
    void printBoardVertex(TGame *game);

    // Print state of Game.
    void printGame(TGame *game);
    
    /*========================================
                GAME FUNCTIONS
    ========================================*/  

    // Print state of board.
    void initializeBoard(TGame *game);

    // Initiating Game dimensions and board.
    void initiateGame(TGame *game);

    /*========================================
                    Painting
    ========================================*/  

    // Free Game structures.
    void freeGame(TGame *game);
    
#endif
