#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Grid.h"
#include "Entity.h"
#include "World.h"
#include "DWGraph.h"

using namespace std;

class Path {
	public:
		vector<Grid> steps;
		int cost;
};
class Node {
public:
	int cost;
	Grid grid;
	Node(Grid _grid, int _cost) { grid = _grid; cost = _cost; }
};

class Pathfinder {
public:

	/*
		IMPORTANT: if either 'World& world' or 'Grid& start' has changed, one should reset first
	*/
	Path* find_path(World& world, UnitComponent& unit, Grid& start, Grid& target);
	void reset() {
		graph = nullptr;
		start = nullptr;
	}

	~Pathfinder() {
		if (graph != nullptr) {
			delete graph;
		}
		if (start != nullptr) {
			//delete start;
		}
	}

private:

	DWGraph* graph;
	Grid* start;

};

#endif
