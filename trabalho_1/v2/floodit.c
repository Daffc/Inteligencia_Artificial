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

    
    // Defining all nodes as not absorbed.
    for (int i = 0; i < graph->V; i++)
        graph->list_nodes[i].absorbed = 0;

    // Reducing memory allocation dimension to the really used.
    graph->list_nodes = realloc(graph->list_nodes, graph->V * sizeof(AdjList));
    if(graph->list_nodes == NULL){
        puts("Error during memory reallocation. Exiting.");
        exit(1);
    }
}
/*========================================
            Solving Problem
========================================*/  


// Search for the farthest node to from origin node '0' 
// and returns a neighbor of '0' that leads to that node. 
int defineNextNode(Graph *graph){    
    BFSTreeNode *BFST;
    int fartest_node, c_node, p_node;

    // Initilize structure to represent BFS Tree and list of visited nodes.
    BFST = (BFSTreeNode *) malloc(graph->V * (sizeof(BFSTreeNode)));

    // Initializing control structures.
    memset(BFST, 0, graph->V * (sizeof(BFSTreeNode)));    

    // Doing BFS and recovering spanning tree in BFST.
    BFS(graph, BFST);

    // printf("BFST:\n");                                                  //DEBUG
    // for(int i = 0; i< graph->V; i++)                                    //DEBUG
    //     printf("\t%d (%d , %d)\n", i, BFST[i].parent, BFST[i].level);   //DEBUG
    // printf(" \n");                                                      //DEBUG


    // Find the farthest node in BFST.
    fartest_node = 0;
    for(int i = 0; i< graph->V; i++){
        if(BFST[i].level > BFST[fartest_node].level)
            fartest_node = i;
    }                                    
    
    // printf("fartest_node: %d\n", fartest_node); //DEBUG


    // Search for the branch of node '0' that leads to the 'fartest_node' 
    // switch between current node (c_node) and parent node (p_node).
    c_node = fartest_node;
    while(p_node = BFST[c_node].parent)
        c_node = p_node;

    // printf("c_node: %d\n", c_node); //DEBUG


    free(BFST);    
    // Returns the chield of '0' that leads to the 'fartest_node'.
    return c_node;
}

// Making node '0' absorbs its neighbors that shares the 
// same color as 'node', changing '0' color as well.
void absorbNodes(Graph *graph, int node){

    AdjListNode *p_next, *p_edges;
    char new_color;
    int neigh_id;

    new_color = graph->list_nodes[node].color;

    graph->list_nodes[0].color = new_color;

    //Check all nodes conected to '0'.
    p_next = graph->list_nodes[0].adj_head;
    while (p_next){
        neigh_id = p_next->dest;

        // If node shares the same color as '0' and hasn't been absorbed.
        if((graph->list_nodes[neigh_id].color == new_color) && (!graph->list_nodes[neigh_id].absorbed)){
            printf("Entrou: %d \n", neigh_id);
            
            //  Transfer all neighbors from 'neigh_id' to '0' (edges).
            p_edges = graph->list_nodes[neigh_id].adj_head;
            while (p_edges){
                if(p_edges->dest){
                    printf("\tAdding: %d\n", p_edges->dest);                    
                    addEdge(graph, 0, p_edges->dest);
                }
                p_edges = p_edges->next;
            }
            // Mark node as absorbed;
            graph->list_nodes[neigh_id].absorbed = 1;
        }
        p_next = p_next->next;
    }
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

    
    // printBoardColors(&game);                    //DEBUG
    // printBoardNode(&game);                      //DEBUG

    

    // Translating current game information to graph.
    fromGameToGraph(&game, &graph);

    // freeing game structures.
    freeGame(&game);
    

    
    printf("=============================\n");  //DEBUG
    // printBoardColors(&game);                    //DEBUG
    // printBoardNode(&game);                      //DEBUG
    // printGraph(&graph);                         //DEBUG
    // printf("\n graph.V: %d\n", graph.V);        //DEBUG
    
    int node;

    node = defineNextNode(&graph);
    absorbNodes(&graph, node);


    

    printGraph(&graph);  

    freeGraph(&graph);
    return 0;
}