#ifndef DWGRAPH_H
#define DWGRAPH_H

using namespace std;

#include <vector>
#include "World.h"
#include "Entity.h"

class DWGraph {

private:

	vector<vector<double>>* costs;
	vector<vector<bool>>* visited;
	int number_of_rows, number_of_columns;

public:

	DWGraph(World& world, Entity& unit) {
		number_of_rows = world.getRows();
		number_of_columns = world.getColumns();

		//Initialize costs matrix
		costs = new vector<vector<double>>;
		for (size_t i = 0; i < world.getRows(); i++) {
			vector<double> row;
			for (size_t j = 0; j < world.getColumns(); j++) {
				row.push_back(INT_MAX);
			}
			costs->push_back(row);
		}

		//Initialize visited matrix
		visited = new vector<vector<bool>>;
		for (size_t i = 0; i < world.getRows(); i++) {
			vector<bool> row;
			for (size_t j = 0; j < world.getColumns(); j++) {
				row.push_back(false);
			}
			visited->push_back(row);
		}
	}

	void set_cost(Grid& g, double cost) {
		(*costs)[g.row][g.col] = cost;
	}

	double get_cost(Grid& g) {
		return (*costs)[g.row][g.col];
	}

	void set_visited(Grid& g, bool _visited) {
		(*visited)[g.row][g.col] = _visited;
	}

	bool is_visited(Grid& g) {
		return (*visited)[g.row][g.col];
	}

	void delete_graph() {
		delete costs;
		delete visited;
	}

	vector<Grid*> neighbours(Grid& g) {

	}
};

#endif