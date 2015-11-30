#ifndef WORLD_H
#define WORLD_H

#include <string>

using namespace std;

class World { 
	public: 
		
		
		size_t getRows();
		size_t getColumns();

		void setRows(int r) { rows = r; }
		void setColumns(int c) { columns = c; }


		// geef cel terug getCell(int row, int col);

		World* generateWorld(string world_file);
		void loadLevel(string world_file);

		

		//bool level_can_walk_over(cell1, cell2)
		
	private: 
		int rows, columns; 
	};

#endif