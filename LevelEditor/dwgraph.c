#include "dwgraph.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

DWGraph* make_graph(Level *level) {
	return make_graph_with_type(level, DEFAULT_CELLTYPE);
}

DWGraph* make_graph_with_type(Level *level, CellType graph_type) {
	int n = level->height;
	int m = level->width;
	Node** nodes = malloc(sizeof(Node*) + (sizeof(Node*)*(n*m)));

	//initialize the nodes
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			Cell *cell = &(level->cells[i][j]);
			Node *node = malloc(sizeof(Node));
			node->cell = cell;
			Pos p;
			p.row = i;
			p.col = j;
			node->pos = p;
			node->visited = false;
			node->cost = INT_MAX;
			node->prev = NULL;

			nodes[(i*m) + j] = node;
		}
	}

	//construct the adjacency list and the correspondings costs list
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			// calculate the row- and columnindex of the neighbouring cells
			int mask[8][2] = {
				{ i - 1,j - 1 },
				{ i + 1,j + 1 },

				{ i - 1,j },
				{ i + 1,j },

				{ i - 1,j + 1 },
				{ i + 1,j - 1 },

				{ i,j - 1 },
				{ i,j + 1 },
			};
			int amount_neighbours = 0;
			Node **neighbours = malloc(sizeof(Node*) * 8);
			Node *node = nodes[(i*m) + j];
			Cell *cell = node->cell;
			int *costs = malloc(sizeof(int)*8);

			for (int x = 0; x < 8; x++) {
				int a = mask[x][0];
				int b = mask[x][1];
				if (level_is_valid_pos(level, a, b)) {
					Cell *neighbour = &(level->cells[a][b]);
					Node *neighbour_node = nodes[(a*m) + b];
					neighbours[amount_neighbours] = neighbour_node;
					costs[amount_neighbours] = calculate_cost(node->cell, neighbour,graph_type);
					amount_neighbours++;
					//	TODO: verbeter de geheugenallocatie van neighbours
					// neighbours = realloc(neighbours, (sizeof(Node*)*amount_neighbours) + sizeof(Node**));
					//	}
				}
			}

			node->amountOfNeighbours = amount_neighbours;
			node->neighbours = neighbours;
			node->costs = costs;
		}
	}

	DWGraph *graph = (DWGraph*)malloc(sizeof(DWGraph));
	graph->graph_type = graph_type;
	graph->nodes = nodes;
	graph->amountOfNodes = n*m;
	graph->amountOfColumns = m;
	return graph;
}

/* Utility method to calculate the cost to move from 1 cell to another.
* Cells must be neighbours in order for this function to work correctly.
* If the celltype of cell is able to move to the celltype of neighbour, then the weight
* of the edge from cell to neighbour is 12,17 if cells positions are respectively horizontal/vertical , diagonal
* (Else the weight is INFINITY.).
* The graph_type needs to be given because the cost does not depend on the level its cells, but the graph itself.
*/
int calculate_cost(Cell *unit, Cell *target,CellType graph_type) {
	/* walk_over only accepts cell-pointers. So we need to make a dummy cell that contains the graph type */
	Cell dummy = *(unit); /*copy, level may not be changed*/
	(&dummy)->type = graph_type;

	if (level_can_walk_over(&dummy, target)) { /* Supposing they are neighbours */
		if (abs(unit->row + unit->col - target->row - target->col) == 1) {
			/* Verschil horizontale, verticale coordinaten is gelijk op een 1-term na.*/
			return 12;
		} else { /* diagonaal */
			return 17;
		}
	} else {
		return INT_MAX;
	}
}



Node* cell_to_node(DWGraph *graph, Cell *cell) {
	int i = cell->row;
	int j = cell->col;
	int m = graph->amountOfColumns;
	return graph->nodes[(i*m) + j];
}


void free_graph(DWGraph *graph) {
	Node **nodes = graph->nodes;
	for (int i = 0; i < graph->amountOfNodes; i++) {
		free(nodes[i]->neighbours);
		free(nodes[i]->costs);
		free(nodes[i]);
	}
	free(nodes);
	free(graph);
}