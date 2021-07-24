// TODO: Remove '//DEBUG' lines.

#include "GameFunctions.h"
#include "Graph.h"
#include <stdio.h>

/*========================================
            Creating Graph
========================================*/  
// Recursive function to paint flooded area while search for borders.
void GenerateGraph(TGame * game, Graph * graph, char prev_color, int row, int col, int node_id){
    if((row < 0) || (col < 0) || (row >= game->rows) || (col >= game->cols))
        return;

    // If color = -1 (alread explored), return.
    if(game->board[row * game->cols + col].color == -1){
        // If it belongs to an area witch node differs from 'node_id', create an edge between the node of both areas.
        if(node_id != game->board[row * game->cols + col].node){
            // printf("node_id: %d, node: %d\n", node_id, game->board[row * game->cols + col].node); //DEBUG
            addEdge(graph, node_id, game->board[row * game->cols + col].node);
        }
        return;
    }

    // If color of cell differs from currente explore area, return.
    if((game->board[row * game->cols + col].color != prev_color))
        return;

    // Change cell color to explored (color = -1) and mak the node witch this cell belongs in the graph.
    game->board[row * game->cols + col].color = -1;
    game->board[row * game->cols + col].node = node_id;

    GenerateGraph(game, graph, prev_color, row, col+1, node_id);
    GenerateGraph(game, graph, prev_color, row, col-1, node_id);
    GenerateGraph(game, graph, prev_color, row+1, col, node_id);
    GenerateGraph(game, graph, prev_color, row-1, col, node_id);
}


// Reading the game's data and board and translate it to a graph where each node will 
// represent a color node in the game board and each adjacent node in the board will represent 
// an edge on the graph.
void fromGameToGraph(TGame * game, Graph * graph){
    int total_board_cels;
    char color;
    void * tmp;
    
    // Auxiliar variable with rows*cols resutl.
    total_board_cels = game->rows * game->cols;

    // Allocating graph node array to the greatest value possible 
    // (case when none of the cells in the board has an adjacent cell of the same color).
    graph->list_nodes = (AdjList *) malloc((total_board_cels * total_board_cels) *sizeof(AdjList));

    // Generating node for the first color region (the one witch the flood operation begins), exploring its region.
    graph->list_nodes[0].color  = game->board[0].color;
    GenerateGraph(game, graph, game->board[0].color, 0, 0, 0);
    graph->V = 1;

    // search whole board for unexplored areas.
    for(int i = 0; i < (total_board_cels); i++){
        // If the current cell of board belongs to an area that has not been yet explored, 
        // create a new node for this area and explore it
        if (game->board[i].color != -1){
            graph->list_nodes[graph->V].adj_head = NULL;
            graph->list_nodes[graph->V].color  = game->board[i].color;
            GenerateGraph(game, graph, game->board[i].color, i / game->cols, i % game->cols, graph->V);
            graph->V += 1;
        }
    }

    // Reducing memory allocation dimension to the really used.
    tmp = realloc(graph->list_nodes, graph->V * sizeof(AdjList));
    if(tmp == NULL){
        puts("Error during memory reallocation. Exiting.");
        exit(1);
    }
}
/*========================================
            Solving Problem
========================================*/  


int defineNextNode(Graph *graph){    
    BFSTreeNode *BFST;

    // Initilize structure to represent BFS Tree and list of visited nodes.
    BFST = (BFSTreeNode *) malloc(graph->V * (sizeof(BFSTreeNode)));

    // Initializing control structures.
    memset(BFST, 0, graph->V * (sizeof(BFSTreeNode)));    

    BFS(graph, BFST);

    printf("BFST:\n");                                                  //DEBUG
    for(int i = 0; i< graph->V; i++)                                    //DEBUG
        printf("\t%d (%d , %d)\n", i, BFST[i].parent, BFST[i].level);   //DEBUG
    printf(" \n");                                                      //DEBUG

    // printf("Queue:");                       //DEBUG
    // queue->actual = queue->head;            //DEBUG
    // while(!checkEmptyQueue(queue))          //DEBUG
    //     printf(" %d", queueGetNext(queue)); //DEBUG
    // printf(" \n");                          //DEBUG

    // printf("Visited:");                 //DEBUG
    // for(int i = 0; i< graph->V; i++)    //DEBUG
    //     printf(" %d", visited[i]);      //DEBUG
    // printf(" \n");                      //DEBUG

    return 0;

} 

/*
TODO: 
1. Busca em largura para procurar vértices mais distante de vértice '0' (origem de floodit).

2. Após encontrar vértice mais distante 'caminhar' em sua direção.
    - Definir todos os vértices como não absorvidos (graph->list_nodes[i].absorbed = 0)
    - loop para resolução.
        - Escolher 'cor' em direção a mais distante. (Busca em largura)
        - Aglutinar todos vértices de mesma cor vizinhos do vértice '0'.
            - Mudar cor de vétice '0'
            - Aglomerar em '0' todos os seus vizinhos que sejam de mesma 'cor'.
                - Adicionar 'cor' a array resposta num_passos ++.
                - Armazenar em list vértices 'aglutinados', adicionar seus vizinhos a vizinhos de '0'.
                - Indicar que vértice foi aglutinado e não 'existe' mais em grafo (graph->list_nodes[i].absorbed = 1)
        - Caso somente vértice '0' sobrando no gráfo, solução.

*/

/*========================================
                MAIN
========================================*/  
int main(){

    

    TGame game;
    Graph graph;

    // Initiating values of game structure.
    initiateGame(&game);
    
    printBoardColors(&game);                    //DEBUG
    printBoardNode(&game);                      //DEBUG

    // Translating current game information to graph.
    fromGameToGraph(&game, &graph);

    // freeing game structures.
    freeGame(&game);


    
    printf("=============================\n");  //DEBUG
    // printBoardColors(&game);                    //DEBUG
    // printBoardNode(&game);                      //DEBUG
    // printGraph(&graph);                         //DEBUG
    // printf("\n graph.V: %d\n", graph.V);        //DEBUG
    
    defineNextNode(&graph);

    return 0;
}