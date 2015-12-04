#ifndef WORLD_H
#define WORLD_H

#include "TextureComponent.h"
#include "UnitComponent.h"
#include "Entity.h"

#include <string>
#include <vector>

#define DEFAULT_TEXTURE Graphics::Sprite::SPRITE_TERRAIN

using namespace std;

class World { 
	public: 
		
		// CONSTRUCTORS AND DESTRUCTOR
		World(string world_file) : world_file(world_file) {
			loadLevel();
		}
		~World();

		size_t getRows();
		size_t getColumns();

		/* Returns all Entities that represent an elemnt of World.
		 * This entity can be a part of the landscape, a unit, a headquarter or
		 * a representation of the player.
		 */
		vector<Entity*> GetWorldEntities();

		void setRows(size_t r) { rows = r; }
		void setColumns(size_t c) { columns = c; }

		/* geef cel terug TODO JENS 
		*
		* 0 = terrein grond
		*En ik lees daaruit wa ik gebruik
		*1 : eenheden en hq
		*2: teams
		*
		*/
		Entity* getCell(int row, int col, int depth);

		TextureComponent* getTextureComponent(char symbol);
		UnitComponent* getUnitComponent(char Symbol);


		

		//bool level_can_walk_over(cell1, cell2)
		
	private: 
		size_t rows, columns;
		const string world_file;
		vector<Entity*> world_entities;

		int isUnit(char s);
		int isHuman(char s);
		int isAI(char s);


		int init_world(ifstream* file);
		bool generateWorld();
		void loadLevel();
	};

#endif