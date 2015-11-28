#include "pathfinder.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <queue>

bool Pathfinder::comp(Grid* i, Grid* j) {
	return graph->get_cost(*i) < graph->get_cost(*j);
}

Path* Pathfinder::find_path(World& world, UnitComponent& unit, Grid& start, Grid& target) {

	graph = new DWGraph(world, unit);
	Grid current = start;
	graph->set_cost(start, 0);

	if (start.col == target.col && start.row == target.row) {
		Path* path = new Path();
		path->steps->col = start.col;
		path->steps->row = start.row;
		graph->delete_graph();
		return path;
	}

	priority_queue<Grid*, vector<Grid*>, decltype(comp)> pqueue(comp);

	pqueue.push(&start);

	while (pqueue.size() > 0) {

		current = *pqueue.top();
		pqueue.pop();

		vector<Grid> neighbours = graph->neighbours(current);
		for (size_t i = 0; i < neighbours.size(); i++) {
			Grid neighbour = neighbours[i];

			if (!graph->is_visited(neighbour)) {
				int c = graph->cost(current,neighbour) == INT_MAX || graph->get_cost(current) == INT_MAX ?
					INT_MAX : graph->get_cost(current) + graph->cost(current, neighbour);
				if (c < 0) c = INT_MAX;
				if (c < graph->get_cost(neighbour)) {
					graph->set_cost(neighbour, c);
					graph->set_previous(neighbour, current);

					pqueue.push(&neighbour);

				}
			}
		}
		graph->set_visited(current, true);
	}

	if (graph->get_cost(target) == INT_MAX) {
		//There is no path
		graph->delete_graph();
		delete graph;
		return NULL;
	}
	else {
		current = target;
		int pathSize = 1;
		while (graph->get_previous(current) != NULL) {
			current = *graph->get_previous(current);
			pathSize++;
		}
		current = target;

		Path* path = new Path();
		path->cost = graph->get_cost(target);

		for (int i = pathSize - 1; i >= 0; i--) {
			path->steps[i] = current;
			current = *graph->get_previous(current);
		}

		graph->delete_graph();
		delete graph;

		return path;
	}
}


