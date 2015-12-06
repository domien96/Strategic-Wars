#include "World.h"
#include "Entity.h"
#include "PositionComponent.h"
#include <iostream>
#include <fstream>

using namespace std;



size_t World::getRows()
{
	return rows;
}

size_t World::getColumns()
{
	return columns;
}

bool World::isUnit(Entity* ent) {
	PositionComponent* pc = dynamic_cast<PositionComponent*>(ent->GetComponent(Component::POSITION));
	return getWorldEntity(pc->pos.row, pc->pos.col, 1) != nullptr;
}

bool World::isHuman(Entity* ent) {
	PositionComponent* pc = dynamic_cast<PositionComponent*>(ent->GetComponent(Component::POSITION));
	Entity* unit_entity = getWorldEntity(pc->pos.row, pc->pos.col, 1);
	if (unit_entity) {
		UnitComponent* uc = dynamic_cast<UnitComponent*>(unit_entity->GetComponent(Component::UNIT));
		return uc->player == 0;

	}
	else {
		return false;
	}
}

bool World::isValidGrid(Grid * grid)
{
	return grid->col < this->columns && grid->col >= 0 &&
		grid->row < this->rows && grid->row >= 0;
}


/* Returns all Entities that represent an elemnt of World at the given depth.
* This entity can be a part of the landscape, a unit, a headquarter or
* a representation of the player.
*/
vector<Entity*> * World::GetWorldEntities(unsigned int depth)
{
	switch (depth) {
		case 0:
			return &world_entities_map[0];
			break;
		case 1:
			return &world_entities_map[1];
			break;
		case 2:
			return &world_entities_map[2];
			break;
		default:
			//laagste diepte om eventueel de anderen niet te overschrijven.
			return &world_entities_map[0];
			break;
	}

	// Waarom niet gwn return &world_entities_map[depth] ?
}

/*
* Returns true if the given character represents a unit or a headquarter
*/
int World::isUnit(char s) {
	return isHuman(s) || isAI(s);
}

/*
* Returns true if the given character represtents a unit or headquarter that is owned by a humpan player
*/
int World::isHuman(char s)
{
	return (s == '1') || (s == '2') || (s == '3') || (s == 'h');
}

/*
* Returns true if the given character represtents a unit or headquarter that is owned by a humpan player
*/
int World::isAI(char s)
{
	return (s == '7') || (s == '8') || (s == '9') || (s == 'H');
}


Entity * World::getWorldEntity(unsigned int row, unsigned int col, unsigned int depth)
{
	//boundaries
	if (row < 0 || row > this->rows - 1 || col < 0 || col > this->columns - 1 || depth <0 || depth>MAX_CELL_DEPTH) {
		return nullptr;
	}
	return world_entities_map[depth].at(row*columns + col);
}

/*
* Generates a texture component using the textual representation of a cell.
* It also generates a texture component for the flag representing the player
*/
TextureComponent* World::getTextureComponent(char symbol) {
	switch (symbol) {
	case '1':
		//infantry, human player
		return new TextureComponent(Graphics::Sprite::SPRITE_INFANTRY);
		break;
	case '2':
		//archery, human player
		return new TextureComponent(Graphics::Sprite::SPRITE_ARCHERY);
		break;
	case '3':
		//firecannon, human player
		return new TextureComponent(Graphics::Sprite::SPRITE_FIRE);
		break;
	case '7':
		//infantry, AI player
		return new TextureComponent(Graphics::Sprite::SPRITE_INFANTRY);
		break;
	case '8':
		//archery, AI player
		return new TextureComponent(Graphics::Sprite::SPRITE_ARCHERY);
		break;
	case '9':
		//firecannon, AI player
		return new TextureComponent(Graphics::Sprite::SPRITE_FIRE);
		break;
	case 'h':
		//headquarter, human player
		return new TextureComponent(Graphics::Sprite::SPRITE_HQ);
		break;
	case 'f':
		//flag, human player
		return new TextureComponent(Graphics::Sprite::SPRITE_FLAG1);
		break;
	case 'H':
		//headquarter, AI player
		return new TextureComponent(Graphics::Sprite::SPRITE_HQ);
		break;
	case 'F':
		//flag, AI player
		return new TextureComponent(Graphics::Sprite::SPRITE_FLAG2);
		break;
	case '*':
		//GROUND
		return new TextureComponent(Graphics::Sprite::SPRITE_TERRAIN);
		break;
	case 'W':
		//WATER
		return new TextureComponent(Graphics::Sprite::SPRITE_WATER);
		break;
	case 'B':
		//BRIDGE
		return new TextureComponent(Graphics::Sprite::SPRITE_BRIDGE);
		break;
	case 'R':
		//ROCK
		return new TextureComponent(Graphics::Sprite::SPRITE_ROCK);
		break;
	default:
		// CURRENT DEFAULT : GROUND
		return new TextureComponent(DEFAULT_TEXTURE);
		break;
	}
}

/*
 * Generates a UnitComponent using the textual representation of a cell. 
 * A unitcomponent has a constructor of the form:
 * UnitComponent(UntiComponent::UnitType _type, int _player, int _hp, int _ap, int _dp, int _range_min, int _range_max)
 * Human player has the number 0 and the AI player has the number 1.
 * The stats of the units we've taken from the assignment.
 * A headquarter can't attack, but it can be attacked, so it has a hp. The hp was not given in the assignment, but we've chosen 15.
 */
UnitComponent* World::getUnitComponent(char symbol) {
		switch (symbol) {
		default:
			//infantry, human player
			return new UnitComponent(UnitComponent::UnitType::INFANTRY, 0, 10, 2, 2, 0, 1);
			break;
		case '1':
			//TODO: wrong system type!

			//infantry, human player
			return new UnitComponent(UnitComponent::UnitType::INFANTRY, 0, 10, 2, 2, 0, 1);
			break;
		case '2':
			//archery, human player
			return new UnitComponent(UnitComponent::UnitType::ARCHERY, 0, 10, 3, 3, 0, 3);
			break;
		case '3':
			//fire, human player
			return new UnitComponent(UnitComponent::UnitType::FIRE, 0, 10, 4, 1, 2, 5);
			break;
		case '7':
			//infantry, AI player
			return new UnitComponent(UnitComponent::UnitType::INFANTRY, 1, 10, 2, 2, 0, 1);
			break;
		case '8':
			//archery, AI player
			return new UnitComponent(UnitComponent::UnitType::ARCHERY, 1, 10, 3, 3, 0, 3);
			break;
		case '9':
			//fire, AI player
			return new UnitComponent(UnitComponent::UnitType::FIRE, 1, 10, 4, 1, 2, 5);
			break;
		case 'h':
			//headquarter, human player
			return new UnitComponent(UnitComponent::UnitType::HQ, 0, 15, 0, 0, 0, 0);
			break;
		case 'H':
			//headquarter, AI player
			return new UnitComponent(UnitComponent::UnitType::HQ, 1, 15, 0, 0, 0, 0);
			break;

		}
}

/*
* Functie die de wereld initialiseert aan de hand van de data in het gegeven TEKSTUEEL bestand.
* Geeft 0 terug indien geslaagd en 1 indien mislukt.
*/
int World::init_world(ifstream* file) {
	string read_width;
	string read_height;
	// The first line is of the form: "width|height"
	getline(*file, read_width, '|');
	getline(*file, read_height);
	// TODO : invalid first line(geen int)
	size_t width = stoi(read_width);
	size_t height = stoi(read_height);
	if (width <= 0 || height <= 0) {
		return 1;
	};
	this->columns = width;
	this->rows = height;

	// Cells initializeren
	int row = -1;
	string line; 

	while (getline(*file,line)) {
		// Volgende rij : Update row counter 
		row++;
		if (line.length() < width) {
			return 1; // te korte lijn
		}

		for (unsigned int col = 0; col < width; col++) {
			char symbol = line.at(col);
			///////////////////////////
			Grid pos = Grid(row, col);

			// depth 0 : Terrain entities voor this grid, elke cell heeft een entity op diepte 0;
			Entity* depth_0 = new Entity();
			depth_0->Add(new PositionComponent(pos, 0));

			// depth 1 : Unit entities voor this grid
			Entity* depth_1 = nullptr; // later initen naargelang de situatie

			// depth 2 : Team/Flag entities voor this grid
			Entity* depth_2 = nullptr; // later initen naargelang de situatie


			if (!isUnit(symbol)) {
				// depth 0 
				TextureComponent* tc = getTextureComponent(symbol);
				depth_0->Add(tc);
				//depth1, depth2 no units or owners defined.
			}
			else {
				depth_1 = new Entity();
				depth_1->Add(new PositionComponent(pos, 1));
				depth_2 = new Entity();
				depth_2->Add(new PositionComponent(pos, 2));

				// depth 0 : gras(default) voor units.
				depth_0->Add(getTextureComponent(DEFAULT_SYMBOL));

				// Depth 1 : Deze entity heeft dan ook een unitcomponent.
				depth_1->Add(getUnitComponent(symbol));
				depth_1->Add(getTextureComponent(symbol));
				// Depth 3 : Vlag

				if (isHuman(symbol)) {
					depth_2->Add(getTextureComponent('f'));
				}
				else {
					depth_2->Add(getTextureComponent('F'));
				}
			}

			world_entities_map[0].push_back(depth_0);
			world_entities_map[1].push_back(depth_1);
			world_entities_map[2].push_back(depth_2);
		}
	}
	if (row != height) {
		return 1;
	}

	return 0;
}

bool World::unit_can_walk_over(Grid from, Grid to) {
	if (this->getWorldEntity(to.row, to.col, 1) != NULL) {
		return false;
	}
	Entity* e = this->getWorldEntity(to.row, to.col, 0);
	TextureComponent* tc = (TextureComponent*) e->GetComponent(Component::TEXTURE);
	Graphics::Sprite sprite = tc->texture;
	return sprite!=Graphics::SPRITE_WATER && sprite!=Graphics::SPRITE_ROCK;
}


/*
* Functie die de file opent en adhv die file de wereld genereert.
* Bovendien zal de wereld ook gecontroleerd worden op geldigheid. <==================TODO TODO TODO TODO TODO TODO
* Return 
* 0 : if everything is ok
* 1 : File cannot be opened or is not readable or accesable
* 2 : Unrecognized extension
* 3 : Described World in the file is not a valid world.
*/
bool World::generateWorld() {
	if (world_file.length() >= 6) {
		/* open() the file */
		ifstream file(world_file);
		if (file.is_open()) {
			string extension = world_file.substr(world_file.find_last_of("."));
			if (extension.compare(".world")==0) {
				//initworld
				// geeft nog een error op de ifstream file
				if (init_world(&file) == 0) {
					file.close();
				    return 0;
				}
				else {
					return 1;
				}
				// if world is not valid
				//		return 3;
			} else {
				// onherkende extensie
				file.close();
				return 2;
			}
		} else {
			file.close();
			return 1;
		}
		file.close();
		return 0;
	}
	else {
		// .world extension can never be the extenson of the file since the filename length < 6.
		return 2;
	}
}

//Entity*

/*
* Functie die men oproept om de wereld te initializeren aan de hand van de opgegeven file.
*/
void World::loadLevel() {
	if (generateWorld() != 0) {
		// loading failed
		// TODO: Fout behandelen
		//throw std::invalid_argument("Level ongeldig");
	}
}

/*
* Destructor
*/
World::~World() {
	// delete all cell-entities
	for (int i = 0; i <= MAX_CELL_DEPTH; i++) {
		for (vector<Entity*>::iterator it = world_entities_map[i].begin(); it != world_entities_map[i].end(); it++) {
			delete *it;
		}
	}
}



