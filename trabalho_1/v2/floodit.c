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
    graph->list_nodes = (GNode *) malloc((total_board_cels * total_board_cels) *sizeof(GNode));

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

    
    // Defining all nodes as not absorbed, except node '0'.
    graph->list_nodes[0].absorbed = 1;
    for (int i = 1; i < graph->V; i++)
        graph->list_nodes[i].absorbed = 0;

    // Reducing memory allocation dimension to the really used.
    graph->list_nodes = realloc(graph->list_nodes, graph->V * sizeof(GNode));
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

    // Find the farthest node in BFST .
    fartest_node = 0;
    for(int i = 0; i< graph->V; i++){
        if(BFST[i].level > BFST[fartest_node].level)
            fartest_node = i;
    }                                    

    // Search for the branch of node '0' that leads to the 'fartest_node' 
    // switch between current node (c_node) and parent node (p_node).
    c_node = fartest_node;
    while(p_node = BFST[c_node].parent)
        c_node = p_node;

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

    // Redefining color of node '0' 
    // (it is optional, but has it's debug and completeness value).
    new_color = graph->list_nodes[node].color;
    graph->list_nodes[0].color = new_color;

    //Check all nodes conected to '0'.
    p_next = graph->list_nodes[0].adj_head;
    while (p_next){
        
        // Storing id of next node.
        neigh_id = p_next->dest;

        // If node shares the same color as '0' and hasn't been absorbed, absorb it.
        if((graph->list_nodes[neigh_id].color == new_color) && (!graph->list_nodes[neigh_id].absorbed)){

            //  Transfer all neighbors from 'neigh_id' to '0' (edges).
            p_edges = graph->list_nodes[neigh_id].adj_head;
            while (p_edges){
                
                // If 'p_edges->dest' is not node '0', Add new edge.
                if(p_edges->dest)                    
                    addEdge(graph, 0, p_edges->dest);
                
                p_edges = p_edges->next;
            }
            // Mark node as absorbed;
            graph->list_nodes[neigh_id].absorbed = 1;
        }
        p_next = p_next->next;
    }
}


// If all nodes of 'graph' had been absorbed, return 1, otherwise return 0.
int checkWin(Graph *graph){

    // Loop through all nodes, if one of them have not been absorbed, return 0.
    for(int i = 0; i< graph->V; i++)
        if(!graph->list_nodes[i].absorbed)
            return 0;

    // Oherwise, the game is over, return 1.
    return 1;
}


// Solve the problem, return the in the variables 'result_steps' (path to result) and 'result_size' (number of steps).
void solveProblem(Graph *graph, char *result_steps, int *result_size){
    int node;

    *result_size = 0;
    // While all nodes haven't been absorbed by node '0'.
    while(!checkWin(graph)){
        
        // Recover the neighbor node of '0' that leads to the farthest node from node '0' in the graph.
        node = defineNextNode(graph);
        
        // Absorbe 'node' as well as all the other neighbors of '0' that share the same color 
        // (directly related to the flood operation in a regular game).   
        absorbNodes(graph, node);
        
        // Add a new color to the path of the result, and contabilize that new step.
        result_steps[*result_size] = graph->list_nodes[node].color;
        *result_size += 1;
    }
}
/*========================================
                MAIN
========================================*/  
int main(){

    char *result_steps;
    int result_size;
    TGame game;
    Graph graph;
    
    // Initiating values of game structure.
    initiateGame(&game); 
    
    // Translating current game information to graph.
    fromGameToGraph(&game, &graph);

    // freeing game structures.
    freeGame(&game);

    // Allocating space to store the steps of the result 
    // (worst case, each step absorbs only one node of the graph).
    result_steps = (char *) malloc(graph.V * sizeof(char));  

    // Solving Problem by the graph perspective.
    solveProblem(&graph, result_steps, &result_size);

    // Printing Final Result.
    // Printing number of steps in solution.
    printf("%d\n", result_size);

    //Printing solution steps.
    for(int i = 0; i < result_size; i++){
        printf("%d ", result_steps[i]);
    }
    printf("\n");

    //Freeing graph structures and 'result_steps'.
    freeGraph(&graph);
    free(result_steps);

    return 0;
}