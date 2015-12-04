#include "DWGraph.h"

#include "TextureComponent.h"

DWGraph::DWGraph(World& world, UnitComponent& unit) {

	this->world = &world;

	number_of_rows = world.getRows();
	number_of_columns = world.getColumns();
	Add(&unit);

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

	//Initialize previous matrix
	previous = new vector<vector<Grid*>>;
	for (size_t i = 0; i < world.getRows(); i++) {
		vector<Grid*> row;
		for (size_t j = 0; j < world.getColumns(); j++) {
			row.push_back(NULL);
		}
		previous->push_back(row);
	}
}

void DWGraph::set_cost(Grid& g, double cost) {
	(*costs)[g.row][g.col] = cost;
}

int DWGraph::get_cost(Grid& g) {
	return (*costs)[g.row][g.col];
}

void DWGraph::set_visited(Grid& g, bool _visited) {
	(*visited)[g.row][g.col] = _visited;
}

bool DWGraph::is_visited(Grid& g) {
	return (*visited)[g.row][g.col];
}

void DWGraph::set_previous(Grid& g, Grid& prev) {
	(*previous)[g.row][g.col] = &prev;
}
Grid* DWGraph::get_previous(Grid& g) {
	return (*previous)[g.row][g.col];
}

vector<Grid> DWGraph::neighbours(Grid& g) {
	vector<Grid> neighbours;
	if (g.row != 0) {
		if (g.col != 0) {
			neighbours.push_back(Grid(g.row - 1, g.col - 1));
		}
		neighbours.push_back(Grid(g.row - 1, g.col));
		if (g.col != number_of_columns - 1) {
			neighbours.push_back(Grid(g.row - 1, g.col + 1));
		}
	}
	if (g.row != number_of_rows - 1) {
		if (g.col != 0) {
			neighbours.push_back(Grid(g.row + 1, g.col - 1));
		}
		neighbours.push_back(Grid(g.row + 1, g.col));
		if (g.col != number_of_columns - 1) {
			neighbours.push_back(Grid(g.row + 1, g.col + 1));
		}
	}
	if (g.col != 0) {
		neighbours.push_back(Grid(g.row, g.col - 1));
	}
	if (g.col != number_of_columns - 1) {
		neighbours.push_back(Grid(g.row, g.col + 1));
	}

	return neighbours;

}

/* Cost of this step, assuming 'from' and 'to' are neighbours */
int DWGraph::cost(Grid& from, Grid& to) {

	int f = from.row * number_of_rows + from.col;
	int t = to.row * number_of_rows + to.col;

	if (!unit_can_walk_over(*world->GetWorldEntities()[f], *world->GetWorldEntities()[t])) {
		return INT_MAX;
	}

	if (abs(from.col - to.col) + abs(from.row - to.row) == 1) {
		return 12;
	}
	else return 17;
}

bool DWGraph::unit_can_walk_over(Entity& unit, Entity& target) {
	UnitComponent* c = (UnitComponent*) target.GetComponent(Component:: UNIT);

	//NEEDS FIX
	return true;
}

DWGraph::~DWGraph() {
	delete visited;
	for (size_t i = 0; i < previous->size(); i++)
	{
		for (size_t j = 0; j < (*previous)[i].size(); j++)
		{
			delete (*previous)[i][j];
		}
	}
	delete previous;
	delete costs;
}