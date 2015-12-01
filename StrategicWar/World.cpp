#include "World.h"
#include "Entity.h"
#include "PositionComponent.h"
#include <iostream>
#include <fstream>

using namespace std;





World::World(string world_file)
{
	this->world_file = world_file;
	loadLevel();
}

size_t World::getRows()
{
	return rows;
}

size_t World::getColumns()
{
	return columns;
}

/*
* Returns true if the given character represents a unit or a headquarter
*/
int World::isUnit(char s) {
	return (s != '*') && (s == 'G') && (s != 'B') && (s != 'R');
}

/*
* Returns true if the given character represtents a unit or headquarter that is owned by a humpan player
*/
int World::isHuman(char s)
{
	return (s == '1') || (s == '2') || (s != '3') || (s != 'h');
}


/*
* Generates a texture component using the textual representation of a cell.
* It also generates a texture component for the flag representing the player
*/
TextureComponent World::getTextureComponent(char symbol) {
	switch (symbol) {
	case '1':
		//infantry, human player
		return TextureComponent(Graphics::Sprite::SPRITE_INFANTRY);
		break;
	case '2':
		//archery, human player
		return TextureComponent(Graphics::Sprite::SPRITE_ARCHERY);
		break;
	case '3':
		//firecannon, human player
		return TextureComponent(Graphics::Sprite::SPRITE_FIRE);
		break;
	case '7':
		//infantry, AI player
		return TextureComponent(Graphics::Sprite::SPRITE_INFANTRY);
		break;
	case '8':
		//archery, AI player
		return TextureComponent(Graphics::Sprite::SPRITE_ARCHERY);
		break;
	case '9':
		//firecannon, AI player
		return TextureComponent(Graphics::Sprite::SPRITE_FIRE);
		break;
	case 'h':
		//headquarter, human player
		return TextureComponent(Graphics::Sprite::SPRITE_HQ);
		break;
	case 'f':
		//flag, human player
		return TextureComponent(Graphics::Sprite::SPRITE_FLAG1);
		break;
	case 'H':
		//headquarter, AI player
		return TextureComponent(Graphics::Sprite::SPRITE_HQ);
		break;
	case 'F':
		//flag, AI player
		return TextureComponent(Graphics::Sprite::SPRITE_FLAG2);
		break;
	case '*':
		//GROUND
		return TextureComponent(Graphics::Sprite::SPRITE_TERRAIN);
		break;
	case 'W':
		//WATER
		return TextureComponent(Graphics::Sprite::SPRITE_WATER);
		break;
	case 'B':
		//BRIDGE
		return TextureComponent(Graphics::Sprite::SPRITE_BRIDGE);
		break;
	case 'R':
		//ROCK
		return TextureComponent(Graphics::Sprite::SPRITE_ROCK);
		break;
	default:
		// CURRENT DEFAULT : GROUND
		return DEFAULT_TEXTURE;
		break;
	}
}

/*
 * Generates a UnitComponent using the textual representation of a cell. 
 * A unitcomponent has a constructor of the form:
 * UnitComponent(System::Type _type, int _player, int _hp, int _ap, int _dp, int _range_min, int _range_max)
 * Human player has the number 0 and the AI player has the number 1.
 * The stats of the units we've taken from the assignment.
 * A headquarter can't attack, but it can be attacked, so it has a hp. The hp was not given in the assignment, but we've chosen 15.
 */
UnitComponent World::getUnitComponent(char symbol) {
		switch (symbol) {
		default:
			//infantry, human player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 0, 10, 2, 2, 0, 1);
			break;
		case '1':
			//TODO: wrong system type!

			//infantry, human player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 0, 10, 2, 2, 0, 1);
			break;
		case '2':
			//archery, human player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 0, 10, 3, 3, 0, 3);
			break;
		case '3':
			//fire, human player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 0, 10, 4, 1, 2, 5);
			break;
		case '7':
			//infantry, AI player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 1, 10, 2, 2, 0, 1);
			break;
		case '8':
			//archery, AI player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 1, 10, 3, 3, 0, 3);
			break;
		case '9':
			//fire, AI player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 1, 10, 4, 1, 2, 5);
			break;
		case 'h':
			//headquarter, human player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 0, 15, 0, 0, 0, 0);
			break;
		case 'H':
			//headquarter, AI player
			return UnitComponent::UnitComponent(System::Type::TYPE_ANIMATION, 1, 15, 0, 0, 0, 0);
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
	int width = stoi(read_width);
	int height = stoi(read_height);
	if (width <= 0 || height <= 0) {
		return 1;
	};
	this->columns = width;
	this->rows = height;

	int counter = 0;
	char symbol; 
	*file >> symbol; // pre-check

	while (!(*file).fail()) {
		// verwerk 
		int r = floor(counter / this->columns);
		int c = counter % this->columns;
		Entity* cell = new Entity();
		Grid grid = Grid::Grid(r, c);
		PositionComponent pc = PositionComponent::PositionComponent(grid, 0);
		cell->Add(&pc);


		// In het geval de cell een unit bevat, gaan we drie entity's boven elkaar hebben: 
		// 1 van de grond onder de unit,  1 van de unit zelf en 1 die de speler aangeeft(met verschillende diepte in de positioncomponent).
		if (!isUnit(symbol)) {
			TextureComponent tc = getTextureComponent(symbol);
			cell->Add(&tc);
		}
		else {
			TextureComponent tc = getTextureComponent('*');
			cell->Add(&tc);

			// Indien er zich op de cell een zekere unit bevindt. Dan maken we hiervoor nog een entity aan, die zich op 
			// dezelfde positie bevindt, maar met een andere diepte. Deze entity heeft dan ook een unitcomponent.
			// Merk op dat ook een hoofdkwartier kan aangevallen worden en bezit wordt door een speler dus dat 
			// deze ook een entity nodig heeft.
			Entity* unit = new Entity();
			PositionComponent pc_unit = PositionComponent::PositionComponent(grid, 1);
			UnitComponent uc_unit = getUnitComponent(symbol);
			TextureComponent tc_unit = getTextureComponent(symbol);
			unit->Add(&pc_unit);
			unit->Add(&uc_unit);
			unit->Add(&tc_unit);

			// Deze units moeten nog een vlag hebben die aangeeft van welke speler ze is.
			// Voor deze vlag maken we nog een entity aan.

			Entity* player = new Entity();
			PositionComponent pc_player = PositionComponent::PositionComponent(grid, 2);
			if (isHuman(symbol)) {
				TextureComponent tc_player = getTextureComponent('f');
				player->Add(&tc_player);
			}

			else {
				TextureComponent tc_player = getTextureComponent('F');
				player->Add(&tc_player);
			}
			player->Add(&pc_player);
		}

		counter++;
		// lees terug een karakter in
		*file >> symbol;
	}

	return 0;
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
			string extension = world_file.substr(world_file.length() - 8);
			if (extension.compare(".world")) {
				//initworld
				// geeft nog een error op de ifstream file
				if (init_world(&file) != 1) {
					file.close();
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


/*
* Functie die men oproept om de wereld te initializeren aan de hand van de opgegeven file.
*/
void World::loadLevel() {
	if (generateWorld() != 0) {
		// loading failed
		// TODO: Fout behandelen
	}
}



