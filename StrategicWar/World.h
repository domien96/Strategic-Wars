#ifndef WORLD_H
#define WORLD_H

#include "TextureComponent.h"
#include "UnitComponent.h"

#include <string>

#define DEFAULT_TEXTURE Graphics::Sprite::SPRITE_TERRAIN

using namespace std;

class World { 
	public: 
		
		World(string world_file) : world_file(world_file) {
			loadLevel();
		}

		int getRows();
		int getColumns();

		void setRows(int r) { rows = r; }
		void setColumns(int c) { columns = c; }


		// geef cel terug getCell(int row, int col);

		TextureComponent getTextureComponent(char symbol);
		UnitComponent getUnitComponent(char Symbol);


		

		//bool level_can_walk_over(cell1, cell2)
		
	private: 
		int rows, columns;
		const string world_file;

		int isUnit(char s);
		int isHuman(char s);

		int init_world(ifstream* file);
		bool generateWorld();
		void loadLevel();
	};

#endif