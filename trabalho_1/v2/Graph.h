// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>

#ifndef GRAPH
#define GRAPH

	// A structure to represent an adjacency list node
	struct AdjListNode
	{
		int dest;
		struct AdjListNode* next;
	};

	// A structure to represent an adjacency list
	struct AdjList
	{
		char color;
		struct AdjListNode *head;
	};

	// A structure to represent a graph. A graph
	// is an adv_vertex of adjacency lists.
	// Size of adv_vertex will be V (number of vertices
	// in graph)
	struct Graph
	{
		int V;
		struct AdjList* adv_vertex;
	};

	// A utility function to create a new adjacency list node
	struct AdjListNode* newAdjListNode(int dest);

	// Adds an edge to an undirected graph
	void addEdge(struct Graph* graph, int src, int dest);

	// A utility function to print the adjacency list
	// representation of graph
	void printGraph(struct Graph* graph);

#endif