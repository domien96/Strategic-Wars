#ifndef DWGRAPH_H
#define DWGRAPH_H

#include "level.h"
#include "common.h"

typedef struct
{
	int todo;
    //TODO define struct
} DWGraph;

typedef struct
{
	Node* neighbours;
	int* costs;
	//TODO
} Node;

Node* cellToNode(Cell* cell);

//TODO define additional functions

#endif
