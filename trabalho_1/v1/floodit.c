
#include "GameFunctions.h"
#include "stdio.h"

/*========================================
                
========================================*/  

// Recursive check areas that compose the border of flooded area.
void countBorder(char* board, int row_lim, int col_lin, char prev_color, int row, int col, int *count){
    

    // If outside of board, return.
    if((row < 0) || (col < 0) || (row >= row_lim) || (col >= col_lin))
        return;

    // If different color, return.
    if((board[row * col_lin + col] != prev_color)){
        return;
    }
    
    // Invalidate current board position.
    board[row * col_lin + col] = -1;
    // Sum presence of the current cel color in counter. 
    count[prev_color - 1] += 1;

    countBorder(board, row_lim, col_lin, prev_color,  row, col+1, count);
    countBorder(board, row_lim, col_lin, prev_color,  row, col-1, count);
    countBorder(board, row_lim, col_lin, prev_color,  row+1, col, count);
    countBorder(board, row_lim, col_lin, prev_color,  row-1, col, count);
}


// Recursive function to paint flooded area while search for borders.
void countChanges(char* board, int row_lim, int col_lin, char prev_color, int row, int col, int *count){

    // If outside of board, return.
    if((row < 0) || (col < 0) || (row >= row_lim) || (col >= col_lin))
        return;

    // If color = -1 (alread verofyed), return.
    if(board[row * col_lin + col] == -1)
        return;

    // If not the same color of flooded area, new border color detected, call countBorder;
    if((board[row * col_lin + col] != prev_color)){
        // Recursive function to count border areas.
        countBorder(board, row_lim, col_lin, board[row * col_lin + col], row, col, count);
        return;
    }

    // Invalidate current board position.
    board[row * col_lin + col] = -1;

    countChanges(board, row_lim, col_lin, prev_color, row, col+1, count);
    countChanges(board, row_lim, col_lin, prev_color, row, col-1, count);
    countChanges(board, row_lim, col_lin, prev_color, row+1, col, count);
    countChanges(board, row_lim, col_lin, prev_color, row-1, col, count);
}

// Checking and returning which color has the maximum value in lexical order.
char getMaxIndex(int *color_array, int size){
    int max = 0;
    for(int i=1; i < size; i++){
        if(color_array[max] < color_array[i]){
            max = i;
        }
    }
    return max;
}

// Function that solves de 'game'.
void greedAreas(TGame *game){
    
    int *counter;
    char *aux_board;
    char max_index, max_color;

    // Allocating colot counter and auxiliar board.
    counter = (int *)  malloc(game->colors * sizeof(int));
    aux_board = (char *) malloc( game->rows * game->cols * sizeof(char));

    // While game has not been solved.
    while(!checkWin(game)){
        
        // Copy game board to auxiliar board.
        memcpy(aux_board, game->board, game->rows * game->cols * sizeof(char));

        // Reset counter array.
        memset(counter, 0, game->colors * sizeof(int));
        
        // Call recursive function to count of main painting.
        countChanges(aux_board, game->rows, game->cols, *aux_board, 0, 0, counter);
        
        // Check which of the colors has the maximum adjacent area(s).
        max_index = getMaxIndex(counter, game->colors);
        max_color = max_index+1;
        
        // Flooding game board with maximum adjacent area color.
        paint(game, game->board[0], max_color, 0, 0);
        
        // String new step towards solution.
        game->result[game->result_size] = max_color;
        game->result_size += 1;
    }

    // Freeing memory allocations.
    free(counter);
    free(aux_board);
}


/*========================================
                MAIN
========================================*/  
int main(){
    TGame game;

    // Initiating values of game structure.
    initiateGame(&game);

    // Solving problem.
    greedAreas(&game);

    // Printing results
    // Printing number of steps of solution.
    printf("%d\n", game.result_size);

    // Printing steps of solution.
    for(int i=0; i < game.result_size; i++){
        printf("%d ", game.result[i]);
    }
    printf("\n");

    // Greeing some game allocations.
    freeGame(&game);

    return 0;
}