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
    
	DWGraph* graph = makeGraph(level);
	int pathLength = 1;

	Node* startNode = cellToNode(graph, start); 
	Node* targetNode = cellToNode(graph, target);
	Node* current = startNode;
	int currentCost = 0;

	PriorityQueue* queue = pqueue_alloc();
	Node **neighbours = (startNode->neighbours);
	for (int i = 0; i < startNode->amountOfNeighbours; i++) {
		pqueue_update(queue, neighbours[i], *(startNode->costs + i));
	}
	
	
	while (sizeof(queue) > 0 && (current->pos.col != targetNode->pos.col || current->pos.row != targetNode->pos.row)) {
		current = pqueue_remove_first(queue);

		Node **neighbours = (current->neighbours);
		for (int i = 0; i < current->amountOfNeighbours; i++) {
			Node *neighbour = neighbours[i];
		
	
			if (!neighbour->visited) {
				int c = current->cost + *(current->costs + i);
				if (c < neighbour->cost) {
					neighbour->cost = c;
					neighbour->prev = current;
				}
				pqueue_update(queue, neighbours[i], c);
			}
		}
		current->visited = true;
	}

	if (targetNode->cost == INT_MAX) {
		//There is no path
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
		Path* path = (Path*)calloc(1, sizeof(Path));
		path->steps = (Pos*)calloc(pathSize, sizeof(Pos));
		for (int i = pathSize-1; i >= 0; i++) {
			*(path->steps + i) = current->pos;
			current = current->prev;
		}
		return path;
	}
}

