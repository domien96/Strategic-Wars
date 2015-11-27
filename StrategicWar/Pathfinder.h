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
public: int cost; Grid grid; vector<Node*> neighbours; bool visited; int* costs; Node* prev;
};
//FIX ME

class Pathfinder {
	public:

		Path* find_path(World& world, Entity& unit, Grid& start, Grid& target);
		vector<Grid> neighbours(Grid& current, int rows, int columns);

};

#endif
