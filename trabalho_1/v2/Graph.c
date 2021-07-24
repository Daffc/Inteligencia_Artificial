// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"


/*========================================
            Graph Manipulation
========================================*/  

// Creates a new element 'AdjListNode' in order to create new edges.
AdjListNode * newAdjListNode(int dest)
{
	AdjListNode * new_node;
	
	// Creating new node with 'dest' value and returning.
	new_node = (AdjListNode *) malloc(sizeof(AdjListNode));
	new_node->dest = dest;
	new_node->next = NULL;
	return new_node;
}

// Adds an edge to the graph.
void addEdge(Graph *graph, int src, int dest)
{

	AdjListNode* new_node;

	// Checks if the edge between 'src' and 'dst' alread exists.
	// If it exists, return.
	AdjListNode* p_next = graph->list_nodes[src].adj_head;
	while (p_next){
		if(p_next->dest == dest)
			return;
		p_next = p_next->next;
	}

	// Defining a edge from 'src' to 'dst'.
	new_node = newAdjListNode(dest);
	new_node->next = graph->list_nodes[src].adj_head;
	graph->list_nodes[src].adj_head = new_node;

	// Defining a edge from 'dst' to 'src'.
	new_node = newAdjListNode(src);
	new_node->next = graph->list_nodes[dest].adj_head;
	graph->list_nodes[dest].adj_head = new_node;
}

// Print Graph by a node perspective.
void printGraph(Graph *graph)
{
	int v;
	for (v = 0; v < graph->V; ++v)
	{
		AdjListNode *p_next = graph->list_nodes[v].adj_head;
		printf("\n vertex %d with color %d \n adj_head ", v, graph->list_nodes[v].color);
		while (p_next)
		{
			printf("-> %d", p_next->dest);
			p_next = p_next->next;
		}
		printf("\n");
	}
}

// Executes a BFS search in 'graph' returning the resulting spanning tree in 'BFST'.
void BFS(Graph *graph, BFSTreeNode *BFST){
    int node;
    Queue *queue;
    char *visited;
    AdjListNode *p_next;
    
    // Initialize queue.
    queue = initializeQueue(graph->V);

    // Initializing control structure.
    visited = (char *) malloc(graph->V * (sizeof(char)));
    memset(visited, 0, graph->V * (sizeof(int)));

    // Add '0' to the queue and mark it as visited
    queuePush(queue, 0);
    *visited = 1;

    //Executing BSF in order to define a minimum spanning tree (BFST).
    // While queue is not empty.
    while(!checkEmptyQueue(queue)){
        // Get Next element of queue
        node = queueGetNext(queue);

        //Check all nodes conected to 'node'
        p_next = graph->list_nodes[node].adj_head;
        while (p_next){
            // If node hasent be visited nor been absorbed.
            if((!visited[p_next->dest]) && (!graph->list_nodes[p_next->dest].absorbed)){
                //Add note to queue.
                queuePush(queue, p_next->dest);

                // Define 'parent' and 'level' of the new explored node in BFST.
                BFST[p_next->dest].level = BFST[node].level + 1;
                BFST[p_next->dest].parent = node;

                // Mark node as visited.
                visited[p_next->dest] = 1;
            }
            p_next = p_next->next;
        }
    }

	free(visited);
	freeQueue(queue);
}


/*========================================
            Queue Manipulation
========================================*/

// Initizes a new queue, alocating 'max_size' space and adjusting pointers.
Queue * initializeQueue(int max_size){
	Queue *queue;
	queue = (Queue *) malloc(sizeof(Queue));
	queue->head = (int *) malloc(max_size * sizeof(int));
	queue->end = queue->actual = queue->head;

	return queue;
}

// Push new 'value' to the end of the queue.
void queuePush(Queue *queue, int value){
    *(queue->end) = value;
    queue->end ++;
}

// Recover value from 'actual' position of the queue, and atualizes it.
int queueGetNext(Queue *queue){
	int value;

	value = *queue->actual;
	queue->actual++;

	return value;
}

// Checks if QUeue is empty (actual == end).
int checkEmptyQueue(Queue *queue){
	return (queue->actual == queue->end);
}

// Deallocate queue memory.
void freeQueue(Queue *queue){
	free(queue->head);
}