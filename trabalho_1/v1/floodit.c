
#include "GameFunctions.h"
#include "stdio.h"

/*========================================
                MAIN
========================================*/  
int main(){
    TGame game;

    initiateGame(&game);


    // ========================================
    //              Some tests.
    // ========================================

    // printGame(&game);

    // paint(&game, game.board[0], 3, 0, 0);
    // printf("%d\n", checkWin(&game));
    // printBoard(&game);

    // paint(&game, game.board[0], 2, 0, 0);
    // printf("%d\n", checkWin(&game));
    // printBoard(&game);

    // paint(&game, game.board[0], 1, 0, 0);
    // printf("%d\n", checkWin(&game));
    // printBoard(&game);

    // paint(&game, game.board[0], 3, 0, 0);
    // printf("%d\n", checkWin(&game));
    // printBoard(&game);

    // paint(&game, game.board[0], 2, 0, 0);
    // printf("%d\n", checkWin(&game));
    // printBoard(&game);

    return 0;
}