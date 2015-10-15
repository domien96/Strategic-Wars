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

	Node* startNode = (graph->nodes) + cellToNode(graph, start);
	Node* targetNode = (graph->nodes) + cellToNode(graph, target);
	Node* current = startNode;
	int currentCost = 0;

	PriorityQueue* queue = pqueue_alloc();
	for (int i = 0; i < sizeof(startNode->neighbours)/sizeof(Node); i++) {
		pqueue_update(queue, startNode->neighbours + i, *(startNode->costs + i));
	}
	
	while (sizeof(queue) > 0 && (current->pos.col != targetNode->pos.col || current->pos.row != targetNode->pos.row)) {
		current = pqueue_remove_first(queue);
		for (int i = 0; i < sizeof(current->neighbours) / sizeof(Node); i++) {
			Node* neighbour = (current->neighbours) + i;
			if (!neighbour->visited) {
				int c = current->cost + *(current->costs + i);
				if (c < neighbour->cost) {
					neighbour->cost = c;
					neighbour->prev = current;
				}
				pqueue_update(queue, (current->neighbours) + i, c);
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

	/*Path* res = path_alloc(100, 0);

    Pos cur;
    cur.row = start->row;
    cur.col = start->col;

    res->step_count = 1;
    res->steps[0] = cur;

    while (cur.row != target->row || cur.col != target->col)
    {
        if (cur.row < target->row) cur.row += 1;
        if (cur.col < target->col) cur.col += 1;
        if (cur.row > target->row) cur.row -= 1;
        if (cur.col > target->col) cur.col -= 1;

        if (res->step_count >= 100)
        {
            free(res->steps);
            free(res);
            return NULL;
        }

        res->steps[res->step_count] = cur;
        res->step_count++;
        res->distance += 12;
    }

    return res;*/
}

