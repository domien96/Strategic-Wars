#ifndef DWGRAPH_H
#define DWGRAPH_H

#include "level.h"
#include "common.h"
#include <stdbool.h>

typedef struct NodeT
{
	struct NodeT* neighbours;
	int* costs; //Costs of edges, in same order as corresponding neighbours.
	bool visited;
	int cost;
	//TODO
} Node;

typedef struct
{
	Node* nodes;
    //TODO define struct
} DWGraph;

//Initialize graph given the level. All used nodes have standard cost +INFINITY and visited is false.
DWGraph* makeGraph(Level* level);

//Returns position in nodes array that corresponds with the cell
int cellToNode(Cell* cell);

//TODO define additional functions

#endif
