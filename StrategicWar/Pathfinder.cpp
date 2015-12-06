#include "pathfinder.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <queue>

/*bool comp(Node* i, Node* j) {
	return i->cost < j->cost;
}*/

Path* Pathfinder::find_path(World& world, UnitComponent& unit, Grid& start, Grid& target) {

	if (start.col == target.col && start.row == target.row) {
		Path* path = new Path();
		path->steps.push_back(start);
		path->cost = 0;
		return path;
	}

	if (this->graph == nullptr && this->start == nullptr) {
		graph = new DWGraph(world, unit);
		this->start = &start;

		Grid current = start;
		graph->set_cost(start, 0);


		queue<Node*> queue;

		queue.push(new Node(start, 0));

		while (queue.size() > 0) {

			current = queue.front()->grid;
			Node* toDelete = queue.front();
			queue.pop();
			delete toDelete;

			vector<Grid> neighbours = graph->neighbours(current);
			for (size_t i = 0; i < neighbours.size(); i++) {
				Grid neighbour = neighbours[i];

				if (!graph->is_visited(neighbour)) {
					int c = graph->cost(current, neighbour) == INT_MAX || graph->get_cost(current) == INT_MAX ?
						INT_MAX : graph->get_cost(current) + graph->cost(current, neighbour);
					if (c < 0) c = INT_MAX;
					if (c < graph->get_cost(neighbour)) {
						graph->set_cost(neighbour, c);
						graph->set_previous(neighbour, current);
						queue.push(new Node(neighbour, c));
					}
				}
			}
			graph->set_visited(current, true);
		}
	}

	if (graph->get_cost(target) == INT_MAX) {
		//There is no path
		return nullptr;
	}
	else {
		Grid current = target;
		int pathSize = 1;
		while (graph->get_previous(current).col != -1) {
			current = graph->get_previous(current);
			pathSize++;
		}
		current = target;

		Path* path = new Path();
		path->cost = graph->get_cost(target);

		for (int i = pathSize - 1; i >= 0; i--) {
			path->steps.push_back(current);
			current = graph->get_previous(current);
		}

		return path;
	}
}


