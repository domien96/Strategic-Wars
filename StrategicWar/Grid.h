#ifndef GRID_H
#define GRID_H

class Grid
{
public:
	unsigned int row;
	unsigned int col;

	bool operator==(const Grid& other) const {
		return row == other.row && col == other.col;
	};

	Grid(unsigned int _row = 0, unsigned int _col = 0):
		row(_row), col(_col) {};

	~Grid()	{};
};

#endif