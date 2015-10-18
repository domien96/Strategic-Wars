#include "dwgraph.h"

#include <stdlib.h>
#include <assert.h>


DWGraph* makeGraph(Level* level) {
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
			//TODO: neighbours* + costs*
		}
	}

	
	DWGraph *graph = (DWGraph*)malloc(sizeof(DWGraph));
	graph->nodes = nodes;
	return graph;
}

Node* cellToNode(DWGraph* graph, Cell* cell) {
	int row = &(cell->row);
	int col = &(cell->col);
	return &(graph->nodes[row][col]);
}

void free_graph(DWGraph* graph) {
	Node **nodes = graph->nodes;
	free(nodes);
	free(graph);
}