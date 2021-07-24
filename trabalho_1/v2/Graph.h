// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>

#ifndef GRAPH
#define GRAPH

	// Element of the list of adjacente nodes.
	typedef struct AdjListNode AdjListNode;
	struct AdjListNode{
		int dest;
		AdjListNode *next;
	};

	// Represents a node in te node list.
	typedef struct AdjList{
		char color;
		char absorbed;
		AdjListNode *adj_head;
	}AdjList;

	// Represents the graph, composed by V nodes, edges could be gathered 
	// by the linked list inside each node in 'list_nodes'.
	typedef struct Graph{
		int V;
		AdjList *list_nodes;
	}Graph;


	// Structure of queue to auxiliate in the BFS processess.
	typedef struct Queue{
		int *head;
		int *actual;
		int *end;
	}Queue;

	// Creates a new element 'AdjListNode' in order to create new edges.
	AdjListNode* newAdjListNode(int dest);

	// Adds an edge to the graph.
	void addEdge(Graph *graph, int src, int dest);

	// Print Graph by a node perspective.
	void printGraph(Graph *graph);

	/*========================================
				Queue Manipulation
	========================================*/

	// Initizes a new queue, alocating 'max_size' space and adjusting pointers.
	Queue * initializeQueue(int max_size);

	// Push new 'value' to the end of the queue.
	void queuePush(Queue *queue, int value);

	// Recover value from 'actual' position of the queue, and atualizes it.
	int queueGetNext(Queue *queue);

	// Checks if QUeue is empty (actual == end).
	int checkEmptyQueue(Queue *queue);

	// Deallocate queue memory.
	void freeQueue(Queue *queue);
#endif