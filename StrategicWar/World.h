#ifndef WORLD_H
#define WORLD_H

#include "TextureComponent.h"
#include "UnitComponent.h"

#include <string>

using namespace std;

class World { 
	public: 
		
		World(string world_file);

		size_t getRows();
		size_t getColumns();

		void setRows(int r) { rows = r; }
		void setColumns(int c) { columns = c; }


		// geef cel terug getCell(int row, int col);

		TextureComponent getTextureComponent(char symbol);
		UnitComponent getUnitComponent(char Symbol);


		

		//bool level_can_walk_over(cell1, cell2)
		
	private: 
		int rows, columns;
		string world_file;

		int init_world(World* world, ifstream file);

		World* generateWorld();
		void loadLevel();
	};

#endif