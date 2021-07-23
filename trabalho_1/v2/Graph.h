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
		AdjListNode *adj_head;
	}AdjList;

	// Represents the graph, composed by V nodes, edges could be gathered 
	// by the linked list inside each node in 'list_nodes'.
	typedef struct Graph{
		int V;
		AdjList *list_nodes;
	}Graph;

	// Creates a new element 'AdjListNode' in order to create new edges.
	AdjListNode* newAdjListNode(int dest);

	// Adds an edge to the graph.
	void addEdge(Graph *graph, int src, int dest);

	// Print Graph by a node perspective.
	void printGraph(Graph *graph);

#endif