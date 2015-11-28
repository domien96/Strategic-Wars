#ifndef WORLD_H
#define WORLD_H

class World { 
	public: 
		size_t getRows();

		size_t getColumns();

		// geef cel terug getCell(int row, int col);

		void loadLevel(string world_file);

		//bool level_can_walk_over(cell1, cell2)
		
	private: 
		int rows, columns; 
	};

#endif