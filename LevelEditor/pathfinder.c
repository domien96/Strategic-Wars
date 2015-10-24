#include "pathfinder.h"

#include "dwgraph.h"
#include "pqueue.h"
#include "level.h"
#include "common.h"
#include "dwgraph.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

Path* find_path(Level* level, Cell* start, Cell* target) {
    
	DWGraph* graph = make_graph_with_type(level, start->type);

	Node* startNode = cell_to_node(graph, start); 
	Node* targetNode = cell_to_node(graph, target);
	Node* current = startNode;
	startNode->cost = 0;

	if (startNode->pos.col == targetNode->pos.col && startNode->pos.row == targetNode->pos.row) {
		Path* path = path_alloc(1, 0);
		path->steps->col = startNode->pos.col;
		path->steps->row = startNode->pos.row;
		free_graph(graph);
		return path;
	}

	PriorityQueue* queue = pqueue_alloc();
	Node **neighbours = (startNode->neighbours);
	for (int i = 0; i < startNode->amountOfNeighbours; i++) {
		int* costs = startNode->costs;
		neighbours[i]->cost = costs[i];
		neighbours[i]->prev = startNode;
		pqueue_update(queue, neighbours[i], costs[i]);
	}
	startNode->visited = true;
	
	while (queue->item_count > 0) {

		current = pqueue_remove_first(queue);

		neighbours = (current->neighbours);
		for (int i = 0; i < current->amountOfNeighbours; i++) {
			Node *neighbour = neighbours[i];
		
			if (!neighbour->visited) {
				int c = current->costs[i] == INT_MAX || current->costs[i] == INT_MAX ?
					INT_MAX : current->cost + current->costs[i];
				if (c < 0) c = INT_MAX;
				if (c < neighbour->cost) {
					neighbour->cost = c;
					neighbour->prev = current;

					pqueue_update(queue, neighbours[i], c);
				}
			}
		}
		current->visited = true;
	}

	if (targetNode->cost == INT_MAX) {
		//There is no path
		pqueue_free(queue);
		free_graph(graph);
		return NULL;
	}
	else {
		current = targetNode;
		int pathSize = 1;
		while (current->prev != NULL) {
			current = current->prev;
			pathSize++;
		}
		current = targetNode;
		Path* path = path_alloc(pathSize, targetNode->cost);
		for (int i = pathSize-1; i >= 0; i--) {
			*(path->steps + i) = current->pos;
			current = current->prev;
		}
		pqueue_free(queue);
		free_graph(graph);
		return path;
	}
}

