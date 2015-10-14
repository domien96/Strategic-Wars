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
	Path* path = path_alloc(1,sizeof(Path));
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
			int c = current->cost + *(current->costs + i);
			if (c < neighbour->cost) {
				neighbour->cost = c;
				path->steps = realloc(path->steps,(pathLength+1)*sizeof(Pos));
				*(path->steps + (pathLength - 1)) = current->pos;
				pathLength++;
			}
			pqueue_update(queue, (current->neighbours) + i, c);
		}
	}

	/*if (targetNode->cost == INT_MAX) {
		//There is no path
		return NULL;
	} else {
		Node* current = startNode;
		*(path->steps) = current->pos;
		while (current->pos.col != targetNode->pos.col && current->pos.row != targetNode->pos.row) {
			Node* neighbours = current->neighbours;
			Node 

		}

	}*/
	
	return 0;
	
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

