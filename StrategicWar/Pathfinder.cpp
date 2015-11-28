#include "pathfinder.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <queue>

bool comp(Node* i, Node* j) {
	return i->cost < j->cost;
}

/*void pqueue_update(priority_queue<Node*, vector<Node*>, decltype(&comp)>* q, void* item, int cost)
{
	assert(q != NULL);
	assert(item != NULL);

	int first_empty = -1;
	for (int i = 0; i < q->item_max_count; i++) {
		if (q->items[i].item == item) {
			q->items[i].cost = cost;
			return;
		}
		if (q->items[i].item == NULL && first_empty < 0)
			first_empty = i;
	}
	if (first_empty < 0) {
		int old_size = q->item_max_count;
		int new_size = q->item_max_count * 2;
		q->items = realloc(q->items, new_size * sizeof(QueueItem));
		q->item_max_count = new_size;
		for (int i = old_size; i < new_size; i++) {
			q->items[i].item = NULL;
		}
		first_empty = old_size;
	}
	assert(first_empty >= 0);
	assert(first_empty < q->item_max_count);
	q->items[first_empty].item = item;
	q->items[first_empty].cost = cost;
	q->item_count++;
}*/

Path* Pathfinder::find_path(World& world, UnitComponent& unit, Grid& start, Grid& target) {

	DWGraph* graph = new DWGraph(world, unit);

	Node* startNode;//= cell_to_node(graph, start);
	Node* targetNode;// = cell_to_node(graph, target);
	Node* current = startNode;
	graph->set_cost(start, 0);

	//if (startNode->pos.col == targetNode->pos.col && startNode->pos.row == targetNode->pos.row) {
	if (start.col == target.col && start.row == target.row) {
		Path* path = new Path();
		path->steps->col = start.col;
		path->steps->row = start.row;
		//delete graph;
		return path;
	}

	priority_queue<Node*, vector<Node*>, decltype(&comp)> pqueue(&comp);

	//FIX ME
	pqueue.push(startNode);
	//pqueue_update(queue, startNode, 0);
	//FIX ME

	while (pqueue.size() > 0) {

		current = pqueue.top();
		pqueue.pop();
		//pqueue_remove_first(queue);

		//vector<Node*> neighbours = current->neighbours; //= Pathfinder::neighbours(current, world.rows, world.columns);

		for (size_t i = 0; i < current->neighbours.size(); i++) {
			Node* neighbour = current->neighbours[i];

			if (!neighbour->visited) {
				int c = current->costs[i] == INT_MAX || current->cost == INT_MAX ?
					INT_MAX : current->cost + current->costs[i];
				if (c < 0) c = INT_MAX;
				if (c < neighbour->cost) {
					neighbour->cost = c;
					neighbour->prev = current;

					//FIX ME
					pqueue.push(current->neighbours[i]);
					//pqueue_update(queue, neighbours[i], c);
					//FIX ME

				}
			}
		}
		current->visited = true;
	}

	if (targetNode->cost == INT_MAX) {
		//There is no path
		while (pqueue.size()!=0) {
			delete pqueue.top();
			pqueue.pop();
		}
		graph->delete_graph();
		delete graph;
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

		Path* path = new Path();
		path->cost = targetNode->cost;

		//path_alloc(pathSize, targetNode->cost);
		for (int i = pathSize - 1; i >= 0; i--) {
			path->steps[i] = current->grid;
			current = current->prev;
		}

		while (pqueue.size()!=0) {
			delete pqueue.top();
			pqueue.pop();
		}
		graph->delete_graph();
		delete graph;

		return path;
	}
}


