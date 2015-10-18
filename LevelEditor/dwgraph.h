#ifndef DWGRAPH_H
#define DWGRAPH_H

#include "level.h"
#include "common.h"
#include <stdbool.h>

typedef struct Node
{
	Cell *cell; //Cell that corresponds to the node
	struct Node **neighbours; //Adjacent nodes
	int amountOfNeighbours; //to make it easier to iterate over the neighbours of the node
	Pos pos; //Corresponding position in grid
	int *costs; //Costs of edges, in same order as corresponding neighbours.
	bool visited; //Initialize as false
	int cost; //Initialize as INT_MAX
	struct Node* prev; //Initialize as NULL
} Node;

typedef struct DWGraph
{
	Node **nodes; //The nodes of the graph. There is no need for an Edge struct, since the information is already contained in  the Node struct
	int amountOfNodes; //to make it easer to iterate over the nodes of the graph
} DWGraph;

//Initialize graph given the level. All used nodes have standard cost +INFINITY and visited is false.
DWGraph* makeGraph(Level *level);

//Returns the node of the graph that corresponds with the given cell in the level grid
Node* cellToNode(DWGraph *graph, Cell* cell);

//Free the allocated space of the given graph
void free_graph(DWGraph *graph);


#endif
