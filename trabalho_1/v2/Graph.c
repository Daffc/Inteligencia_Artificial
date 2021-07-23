// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

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
	while (p_next)
	{
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