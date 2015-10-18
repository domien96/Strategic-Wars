#ifndef DWGRAPH_H
#define DWGRAPH_H

#include "level.h"
#include "common.h"
#include <stdbool.h>

typedef struct Node
{
	Cell* cell; //Cell that corresponds to the node
	struct Node *neighbours; //Adjacent nodes
	Pos pos; //Corresponding position in grid
	int *costs; //Costs of edges, in same order as corresponding neighbours.
	bool visited; //Initialize as false
	int cost; //Initialize as INT_MAX
	struct Node* prev; //Initialize as NULL
} Node;

typedef struct DWGraph
{
	Node **nodes;
} DWGraph;

//Initialize graph given the level. All used nodes have standard cost +INFINITY and visited is false.
DWGraph* makeGraph(Level* level);

//Returns position in nodes array that corresponds with the cell
Node* cellToNode(DWGraph* graph, Cell* cell);


//TODO define additional functions

#endif
