#ifndef DWGRAPH_H
#define DWGRAPH_H

using namespace std;

#include <vector>
#include "World.h"
#include "Entity.h"
#include "UnitComponent.h"

class DWGraph: public Entity {

private:

	vector<vector<double>>* costs;
	vector<vector<bool>>* visited;
	int number_of_rows, number_of_columns;

public:

	DWGraph(World& world, UnitComponent& unit);
	void set_cost(Grid& g, double cost);
	double get_cost(Grid& g);
	void set_visited(Grid& g, bool _visited);
	bool is_visited(Grid& g);
	void delete_graph();
	vector<Grid> neighbours(Grid& g);
	int cost(Grid& from, Grid& to);
};

#endif