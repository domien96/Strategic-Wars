#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Grid.h"
#include "Entity.h"
#include "World.h"
#include "DWGraph.h"

using namespace std;

//FIX ME
class Path { public: Grid* steps; int cost; };

/*class Node {
public: int cost; Grid grid; vector<Node*> neighbours; bool visited; int* costs; Node* prev;
};*/
//FIX ME

class Pathfinder {
public:

	bool comp(Grid* i, Grid* j);
	//DWGraph* get_graph();
	Path* find_path(World& world, UnitComponent& unit, Grid& start, Grid& target);

private:

	DWGraph* graph;

};

#endif
