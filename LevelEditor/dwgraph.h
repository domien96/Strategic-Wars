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
	int amountOfColumns;
	CellType graph_type; /* For more information , see "make_graph"-function */
} DWGraph;

/* Initialize graph given the level. All used nodes have standard cost +INFINITY and visited is false.
 * Since the graph-edges differs for each CellType, the user of this function needs to give a CellType.
 * So for each kind of CellType, the type of the graph will differ also because some CellTypes may
 * cross a certain set of cells, when other celltypes cannot cross this set of cells.
 * For example: Headquarters may cross units, but units may not cross units.
 */
DWGraph* make_graph(Level *level, CellType graph_type);

/* Utility method to calculate the cost to move from 1 cell to another.
 * Cells must be neighbours in order for this function to work correctly.
 * If the celltype of cell is able to move to the celltype of neighbour, then the weight
 * of the edge from cell to neighbour is 12,17 if cells positions are respectively horizontal/vertical , diagonal 
 * (Else the weight is INFINITY.). 
 * The graph_type needs to be given because the cost does not depend on the level its cells, but the graph itself.
 */
int calculate_cost(Cell* from, Cell* to, CellType graph_type);

//Returns the node of the graph that corresponds with the given cell in the level grid
Node* cell_to_node(DWGraph *graph, Cell* cell);

//Updates the graph after a cell on the level grid has changed
void update_graph(DWGraph *graph, Cell *cell);

//Free the allocated space of the given graph
void free_graph(DWGraph *graph);


#endif
