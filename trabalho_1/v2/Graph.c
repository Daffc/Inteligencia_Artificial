// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
	struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest)
{
	// Add an edge from src to dest. A new node is
	// added to the adjacency list of src. The node
	// is added at the beginning

	struct AdjListNode* pCrawl = graph->adv_vertex[src].head;
	while (pCrawl)
	{
		if(pCrawl->dest == dest)
			return;
		pCrawl = pCrawl->next;
	}

	struct AdjListNode* newNode = newAdjListNode(dest);
	newNode->next = graph->adv_vertex[src].head;
	graph->adv_vertex[src].head = newNode;

	// Since graph is undirected, add an edge from
	// dest to src also
	newNode = newAdjListNode(src);
	newNode->next = graph->adv_vertex[dest].head;
	graph->adv_vertex[dest].head = newNode;
}

// A utility function to print the adjacency list
// representation of graph
void printGraph(struct Graph* graph)
{
	int v;
	for (v = 0; v < graph->V; ++v)
	{
		struct AdjListNode* pCrawl = graph->adv_vertex[v].head;
		printf("\n vertex %d with color %d \n head ", v, graph->adv_vertex[v].color);
		while (pCrawl)
		{
			printf("-> %d", pCrawl->dest);
			pCrawl = pCrawl->next;
		}
		printf("\n");
	}
}