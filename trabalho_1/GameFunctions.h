#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GAME_FUNCTIONS
#define GAME_FUNCTIONS

    typedef struct t_Game{
        char *board;
        int rows;
        int cols;
        int colors;
    } TGame;

    // Print state of board.
    void printBoard(TGame *game);

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

    // Recursive function to paint the board.
    void paint(TGame * game, char prev_color, char next_color, int row, int col);

    // Check if the game is complete (board is all in the same color).
    char checkWin(TGame * game);
    
#endif
