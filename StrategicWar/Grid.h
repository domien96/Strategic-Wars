#ifndef GRID_H
#define GRID_H

class Grid
{
public:
	int row;
	int col;

	bool operator==(const Grid& other) const {
		return row == other.row && col == other.col;
	};

	Grid(int _row = 0, int _col = 0):
		row(_row), col(_col) {};

	~Grid()	{};
};

#endif