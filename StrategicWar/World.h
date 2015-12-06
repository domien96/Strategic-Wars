#ifndef WORLD_H
#define WORLD_H

#include "TextureComponent.h"
#include "UnitComponent.h"
#include "Entity.h"

#include <string>
#include <vector>

#define DEFAULT_SYMBOL '*'
#define DEFAULT_TEXTURE Graphics::Sprite::SPRITE_TERRAIN
#define MAX_CELL_DEPTH 3

using namespace std;

class World { 
	public: 
		
		// CONSTRUCTORS AND DESTRUCTOR
		World(string world_file) : world_file(world_file) {
			for (int i = 0; i <= MAX_CELL_DEPTH; i++) {
				world_entities_map[i] = vector<Entity*>();
			}
			loadLevel();
		}
		~World();

		size_t getRows();
		size_t getColumns();

		/* Returns all Entities that represent an elemnt of World at the given depth.
		 * This entity can be a part of the landscape, a unit, a headquarter or
		 * a representation of the player.
		 *
		 * Depth 0 : Entities about the kind of terrain that their Cell has. Never nullpointer because each grid has a background.
		 * Depth 1 : Entities about the unit standing on this cell, which may be nullpointer if no unit is standing there.
		 * Depth 2 : Entities about the team where this Cell belongs to, may also be nullpointer when this Cell has no owner.
		 *
		 */
		vector<Entity*> *GetWorldEntities(unsigned int depth);

		void setRows(size_t r) { rows = r; }
		void setColumns(size_t c) { columns = c; }

		/* geef cel terug  op coordinaten x,y,z.
		* indien er niets op de specifieke diepte is, return null
		* indien x of y buiten grid vallen, wordt entity op rand weergegeven
		*
		* 0 = terrein grond
		*En ik lees daaruit wa ik gebruik
		*1: eenheden en hq
		*2: teams
		*3: grid selector
		*4: health bars
		*
		*/
		Entity* getWorldEntity(unsigned int row, unsigned int col, unsigned int depth);

		TextureComponent* getTextureComponent(char symbol);
		UnitComponent* getUnitComponent(char Symbol);
		

		bool unit_can_walk_over(Grid from, Grid to);
		
	private: 
		// No World creation without filename.
		World();

		size_t rows, columns;
		const string world_file;

		/* Geeft de entities terug per celldiepte */
		map<unsigned int, vector<Entity*>> world_entities_map;

		int isUnit(char s);
		int isHuman(char s);
		int isAI(char s);


		int init_world(ifstream* file);
		bool generateWorld();
		void loadLevel();
	};

#endif