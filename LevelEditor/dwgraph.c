#include "dwgraph.h"

#include <stdlib.h>
#include <assert.h>


DWGraph* makeGraph(Level *level) {
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
			int *costs;

			for (int x = 0; x < 8; x++) {
				int a = mask[x][0];
				int b = mask[x][1];
				if (level_is_valid_pos(level, a, b)) {
					Cell *neighbour = &(level->cells[a, b]);
					Node *neighbour_node = nodes[(a*m) + b];
					neighbours[amount_neighbours] = neighbour_node;
					costs[amount_neighbours] = calculate_cost(node, neighbour);
					amount_neighbours++;
					//	TODO: verbeter de geheugenallocatie van neighbours
					//neighbours = realloc(neighbours, (sizeof(Node*)*amount_neighbours) + sizeof(Node**));
					//	}
				}
			}

			node->amountOfNeighbours = amount_neighbours;
			node->neighbours = neighbours;

		}
	}

	DWGraph *graph = (DWGraph*)malloc(sizeof(DWGraph));
	graph->nodes = nodes;
	graph->amountOfNodes = n*m;
	return graph;
}

/* Utility method to calculate the cost to move from 1 cell to another.
If the celltype of cell is able to move to the celltype of neighbour, then the weight
of the edge from cell to neighbour is 1. Else the weight is INFINITY.
*/
int calculate_cost(Cell *unit, Cell *target) {
	if (level_can_walk_over(unit, target)) {
		return 1;
	}
	else {
		return INT_MAX;
	}
}



Node* cellToNode(DWGraph *graph, Cell *cell) {
	int row = &(cell->row);
	int col = &(cell->col);
	return &(graph->nodes[row][col]);
}

void updateGraph(DWGraph *graph, Cell *cell) {
	Node *node = cellToNode(graph, cell);
	node->cell = cell;
	Node **neighbours = (node->neighbours);
	int* costs = (node->costs);
	for (int i = 0; i < node->amountOfNeighbours; i = i++) {
		// de kost van de node naar zijn neighbours aanpassen
		Node *neighbour = neighbours[i];
		costs[i] = calculate_cost(node, neighbour);

		// de kost van de neighbours naar de node aanpassen
		// TODO!!!
		// ?? = calculate_cost(neighbour, node);

	}

}

void free_graph(DWGraph *graph) {
	Node **nodes = graph->nodes;
	for (int i = 0; i < graph->amountOfNodes; i++) {
		free(nodes[i]->neighbours);
		free(nodes[i]);
	}
	free(nodes);
	free(graph);
}