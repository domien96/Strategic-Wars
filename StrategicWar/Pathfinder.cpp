#include "pathfinder.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <queue>


//FIX ME
class Path { public: Grid* steps; int cost; };
class World;
class Cell;
class Node { public: int cost; Grid grid; Node** neighbours; size_t amountOfNeighbours; bool visited; int* costs; Node* prev; };
class DWGraph;
//FIX ME


using namespace std;

Path* /*Pathfinder::*/find_path(World* world, Cell* start, Cell* target) {

	//DWGraph* graph = make_graph_with_type(level, start->type);

	Node* startNode /* = cell_to_node(graph, start)*/;
	Node* targetNode /*= cell_to_node(graph, target)*/;
	Node* current = startNode;
	startNode->cost = 0;

	//if (startNode->pos.col == targetNode->pos.col && startNode->pos.row == targetNode->pos.row) {
	if (startNode->grid.col == targetNode->grid.col && startNode->grid.row == targetNode->grid.row) {
		Path* path = new Path();
		path->steps->col = startNode->grid.col;
		path->steps->row = startNode->grid.row;
		//remove graph;
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

		Node** neighbours = (current->neighbours);
		for (size_t i = 0; i < current->amountOfNeighbours; i++) {
			Node *neighbour = neighbours[i];

			if (!neighbour->visited) {
				int c = current->costs[i] == INT_MAX || current->costs[i] == INT_MAX ?
					INT_MAX : current->cost + current->costs[i];
				if (c < 0) c = INT_MAX;
				if (c < neighbour->cost) {
					neighbour->cost = c;
					neighbour->prev = current;

					//FIX ME
					pqueue->push(neighbours[i]);
					//pqueue_update(queue, neighbours[i], c);
					//FIX ME

				}
			}
		}
		current->visited = true;
	}

	if (targetNode->cost == INT_MAX) {
		//There is no path
		delete pqueue;
		//pqueue_free(queue);
		//free_graph(graph);
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
		delete pqueue;
		//pqueue_free(queue);
		//free_graph(graph);
		return path;
	}
}
