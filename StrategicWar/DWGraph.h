#ifndef DWGRAPH_H
#define DWGRAPH_H

using namespace std;

#include <vector>
#include "World.h"
#include "Entity.h"

class DWGraph {
public:
	vector<vector<double>>* graph;
	int number_of_rows, number_of_columns;

	DWGraph(World& world, Entity& unit) {
		number_of_rows = world.rows;
		number_of_columns = world.columns;
		graph = new vector<vector<double>>;
		for (size_t i = 0; i < world.rows; i++) {
			vector<double> row;
			for (size_t j = 0; j < world.columns; j++) {
				row.push_back(INT_MAX);
			}
			graph->push_back(row);
		}
	}

	void set(Grid& g, double cost) {
		(*graph)[g.row][g.col] = cost;
	}

	double get_cost(Grid& g) {
		return (*graph)[g.row][g.col];
	}
};

#endif