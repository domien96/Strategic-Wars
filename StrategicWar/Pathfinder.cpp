#include "pathfinder.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <queue>




Path* Pathfinder::find_path(World& world, Entity& unit, Grid& start, Grid& target) {

	DWGraph* graph = new DWGraph(world, unit);

	Node* startNode;//= cell_to_node(graph, start);
	Node* targetNode;// = cell_to_node(graph, target);
	Node* current = startNode;
	graph->set(start, 0);

	//if (startNode->pos.col == targetNode->pos.col && startNode->pos.row == targetNode->pos.row) {
	if (start.col == target.col && start.row == target.row) {
		Path* path = new Path();
		path->steps->col = start.col;
		path->steps->row = start.row;
		//delete graph;
		return path;
	}

	priority_queue<Node*>* pqueue = new priority_queue<Node*>();

	//FIX ME
	pqueue->push(startNode);
	//pqueue_update(queue, startNode, 0);
	//FIX ME

	while (pqueue->size() > 0) {

		current = pqueue->top();
		pqueue->pop();
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
					pqueue->push(current->neighbours[i]);
					//pqueue_update(queue, neighbours[i], c);
					//FIX ME

				}
			}
		}
		current->visited = true;
	}

	if (targetNode->cost == INT_MAX) {
		//There is no path
		while (pqueue->size()!=0) {
			delete pqueue->top();
			pqueue->pop();
		}
		delete pqueue;
		delete graph->graph;
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

		while (pqueue->size()!=0) {
			delete pqueue->top();
			pqueue->pop();
		}
		delete pqueue;
		delete graph->graph;
		delete graph;

		return path;
	}
}

vector<Grid> Pathfinder::neighbours(Grid& current, int rows, int columns) {
	vector<Grid> neighbours;
	if (current.row != 0) {
		if (current.col != 0) {
			neighbours.push_back(Grid(current.row - 1, current.col - 1));
		}
		neighbours.push_back(Grid(current.row - 1, current.col));
		if (current.col != columns - 1) {
			neighbours.push_back(Grid(current.row - 1, current.col + 1));
		}
	}
	if (current.row != rows - 1) {
		if (current.col != 0) {
			neighbours.push_back(Grid(current.row + 1, current.col - 1));
		}
		neighbours.push_back(Grid(current.row + 1, current.col));
		if (current.col != columns - 1) {
			neighbours.push_back(Grid(current.row + 1, current.col + 1));
		}
	}
	if (current.col != 0) {
		neighbours.push_back(Grid(current.row, current.col - 1));
	}
	if (current.col != columns - 1) {
		neighbours.push_back(Grid(current.row, current.col + 1));
	}

	return neighbours;
}
