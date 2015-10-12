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
	Path* path = path_alloc(0,0);

	Node* startNode = (graph->nodes) + cellToNode(start);
	Node* targetNode = (graph->nodes) + cellToNode(target);
	int currentCost = 0;

	PriorityQueue* queue = pqueue_alloc();
	for (int i = 0; i < sizeof(startNode->neighbours)/sizeof(Node); i++) {
		pqueue_update(queue, startNode->neighbours + i, *(startNode->costs + i));
	}
	
	while (sizeof(queue) > 0) {
		Node* current = pqueue_remove_first(queue);
		for (int i = 0; i < sizeof(current->neighbours) / sizeof(Node); i++) {
			Node* neighbour = (current->neighbours) + i;
			neighbour->cost = current->cost + *(current->costs + i);
			pqueue_update(queue, (current->neighbours) + i, neighbour->cost);
		}
	}

	if (targetNode->cost == +INT_MAX) {
		//There is no path
	} else {

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

