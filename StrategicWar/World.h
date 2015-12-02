#ifndef WORLD_H
#define WORLD_H

#include "TextureComponent.h"
#include "UnitComponent.h"
#include "PositionComponent.h"
#include "Entity.h"

#include <string>
#include <vector>

#include <iostream>
#include <fstream>


#define DEFAULT_TEXTURE Graphics::Sprite::SPRITE_TERRAIN

using namespace std;

class World { 
	public: 
		
		// CONSTRUCTORS AND DESTRUCTOR
		World(string world_file) : world_file(world_file) {
			loadLevel();
		}
		~World();

		int getRows();
		int getColumns();

		/* Returns all Entities that represent an elemnt of World.
		 * This entity can be a part of the landscape, a unit, a headquarter or
		 * a representation of the player.
		 */
		vector<Entity*> GetWorldEntities();

		void setRows(int r) { rows = r; }
		void setColumns(int c) { columns = c; }




		// geef cel terug getCell(int row, int col);

		TextureComponent* getTextureComponent(char symbol);
		UnitComponent* getUnitComponent(char Symbol);


		

		//bool level_can_walk_over(cell1, cell2)
		
	private: 
		int rows, columns;
		const string world_file;
		vector<Entity*> world_entities;

		int isUnit(char s);
		int isHuman(char s);

		int init_world(ifstream* file);
		bool generateWorld();
		void loadLevel();
	};

#endif