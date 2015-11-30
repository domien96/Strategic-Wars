#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Grid.h"
#include "Entity.h"
#include "World.h"
#include "DWGraph.h"

using namespace std;

//FIX ME
class Path { public: Grid* steps; int cost; };
class Node {
public:
	int cost;
	Grid grid;
	Node(Grid _grid, int _cost) { grid = _grid; cost = _cost; }
};
//FIX ME

class Pathfinder {
public:

	Path* find_path(World& world, UnitComponent& unit, Grid& start, Grid& target);

private:

	DWGraph* graph;

};

#endif
