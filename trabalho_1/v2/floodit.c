
#include "GameFunctions.h"
#include "Graph.h"
#include "stdio.h"

/*========================================
                
========================================*/  
// Recursive function to paint flooded area while search for borders.
void GenerateGraph(TGame * game, struct Graph * graph, char prev_color, int row, int col, int vertex_id){
    if((row < 0) || (col < 0) || (row >= game->rows) || (col >= game->cols))
        return;

    // If color = -1 (alread explored), return.
    if(game->board[row * game->cols + col].color == -1){
        // If it belongs to an area witch vertex differs from 'vertex_id', create an edge between the vertex of both areas.
        if(vertex_id != game->board[row * game->cols + col].vertex){
            // printf("vertex_id: %d, vertex: %d\n", vertex_id, game->board[row * game->cols + col].vertex); //DEBUG
            addEdge(graph, vertex_id, game->board[row * game->cols + col].vertex);
        }
        return;
    }

    // If color of cell differs from currente explore area, return.
    if((game->board[row * game->cols + col].color != prev_color))
        return;

    // Change cell color to explored (color = -1) and mak the vertex witch this cell belongs in the graph.
    game->board[row * game->cols + col].color = -1;
    game->board[row * game->cols + col].vertex = vertex_id;

    GenerateGraph(game, graph, prev_color, row, col+1, vertex_id);
    GenerateGraph(game, graph, prev_color, row, col-1, vertex_id);
    GenerateGraph(game, graph, prev_color, row+1, col, vertex_id);
    GenerateGraph(game, graph, prev_color, row-1, col, vertex_id);
}


// Reading the game's data and board and translate it to a graph where each vertex will 
// represent a color node in the game board and each adjacent node in the board will represent 
// an edge on the graph.
void fromGameToGraph(TGame * game, struct Graph * graph){
    int total_board_cels;
    char color;
    void * tmp;
    
    // Auxiliar variable with rows*cols resutl.
    total_board_cels = game->rows * game->cols;

    // Allocating graph vertex array to the greatest value possible 
    // (case when none of the cells in the board has an adjacent cell of the same color).
    graph->adv_vertex = (struct AdjList*) malloc((total_board_cels * total_board_cels) *sizeof(struct AdjList));

    // Generating vertex for the first color region (the one witch the flood operation begins), exploring its region.
    graph->adv_vertex[0].color  = game->board[0].color;
    GenerateGraph(game, graph, game->board[0].color, 0, 0, 0);
    graph->V = 1;

    // search whole board for unexplored areas.
    for(int i = 0; i < (total_board_cels); i++){
        // If the current cell of board belongs to an area that has not been yet explored, 
        // create a new vertex for this area and explore it
        if (game->board[i].color != -1){
            graph->adv_vertex[graph->V].head = NULL;
            graph->adv_vertex[graph->V].color  = game->board[i].color;
            GenerateGraph(game, graph, game->board[i].color, i / game->cols, i % game->cols, graph->V);
            graph->V += 1;
        }
    }

    // Reducing memory allocation dimension to the really used.
    tmp = realloc(graph->adv_vertex, graph->V * sizeof(struct AdjList));
    if(tmp == NULL){
        puts("Error during memory reallocation. Exiting.");
        exit(1);
    }


}

/*========================================
                MAIN
========================================*/  
int main(){

    

    TGame game;
    struct Graph graph;

    // Initiating values of game structure.
    initiateGame(&game);
    
    printBoardColors(&game);                    //DEBUG
    printBoardVertex(&game);                    //DEBUG

    // Translating current game information to graph.
    fromGameToGraph(&game, &graph);

    // freeing game structures.
    freeGame(&game);


    
    printf("=============================\n");  //DEBUG
    printBoardColors(&game);                    //DEBUG
    printBoardVertex(&game);                    //DEBUG
    printGraph(&graph);                         //DEBUG
    printf("\n graph.V: %d\n", graph.V);        //DEBUG

    return 0;
}