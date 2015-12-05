#ifndef WORLD_H
#define WORLD_H

#include "TextureComponent.h"
#include "UnitComponent.h"
#include "Entity.h"

#include <string>
#include <vector>

#define DEFAULT_SYMBOL '*'
#define DEFAULT_TEXTURE Graphics::Sprite::SPRITE_TERRAIN
#define MAX_CELL_DEPTH 2

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
		 * Depth 0 : Entities about the kind of terrain that their Cell has.
		 * Depth 1 : Entities about the unit standing on this cell, which may be null if no unit is standing there.
		 * Depth 2 : Entities about the team where this Cell belongs to, may also be null when this Cell has no owner.
		 *
		 */
		vector<Entity*> GetWorldEntities(unsigned int depth);

		void setRows(size_t r) { rows = r; }
		void setColumns(size_t c) { columns = c; }

		/* geef cel terug  op coordinaten x,y,z.TODO JENS 
		*
		* 0 = terrein grond
		*En ik lees daaruit wa ik gebruik
		*1 : eenheden en hq
		*2: teams
		*
		*/
		Entity* getWorldEntity(int row, int col, int depth);

		TextureComponent* getTextureComponent(char symbol);
		UnitComponent* getUnitComponent(char Symbol);


		

		//bool level_can_walk_over(cell1, cell2)
		
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