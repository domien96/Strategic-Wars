#include "World.h"
#include "Entity.h"
#include "PositionComponent.h"
#include <iostream>
#include <fstream>

using namespace std;


/*

void init_texturemap() {
std::map<char, TextureComponent*> = textures
textures['*'] = *TextureComponent(Graphics::Sprite::SPRITE_WATER);
//textures['W'] = TextureComponent(Graphics::Sprite::SPRITE_WATER);
//textures['B'] = TextureComponent(Graphics::Sprite::SPRITE_BRIDGE);
//textures['R'] = TextureComponent(Graphics::Sprite::SPRITE_ROCK);
//textures['H'] = TextureComponent(Graphics::Sprite::SPRITE_HQ);
*/

World::World(string world_file)
{
	this->world_file = world_file;
	loadLevel();
}

/*
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
int World::init_world(World* world, ifstream file) {
	string read_width;
	string read_height;
	// The first line is of the form: "width|height"
	getline(file, read_width, '|');
	getline(file, read_height);
	// TODO : invalid first line(geen int)
	int width = stoi(read_width);
	int height = stoi(read_height);
	if (width <= 0 || height <= 0) {
		return 1;
	};
	world->setColumns(width);
	world->setRows(height);

	// Hou alle karakters bij die aangeven dat een symbool een unit is
	std::vector<char> unitchars;
	unitchars.push_back('1');
	unitchars.push_back('2');
	unitchars.push_back('3');
	unitchars.push_back('7');
	unitchars.push_back('8');
	unitchars.push_back('9');

	//initialize cells
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			// Read each charachter: TODO
			char symbol; // !!!!
			// Een cell is een entity met een positioncomponent en een texturecomponent.
			Grid grid = Grid::Grid(r, c);
			PositionComponent pc = PositionComponent::PositionComponent(grid, 0);
			TextureComponent tc = getTextureComponent(symbol);
			Entity* cell = new Entity();
			cell->Add(&pc);

			// Indien er zich op de cell een zekere unit bevindt. Dan maken we hiervoor nog een entity aan, die zich op 
			// dezelfde positie bevindt, maar met een andere diepte. Deze entity heeft dan ook een unitcomponent
			// Check of het symbool een unit voorstelt
			std::vector<char>::iterator it;
			it = find(unitchars.begin(), unitchars.end(), symbol);
			if (it != unitchars.end()) {
				Entity* unit = new Entity();
				PositionComponent pc_unit = PositionComponent::PositionComponent(grid, 1);
				UnitComponent uc_unit = getUnitComponent(symbol);
				unit->Add(&pc_unit);
			}

			// Voor een hoofdkwartier maken we geen nieuwe entity aan omdat een hoofdkwartier zich toch niet zal 
			// verplaatsen, maar een hoofdkwartier heeft wel een extra unitcomponent nodig, omdat het bezit wordt 
			// door een speler.
			// Check of het symbool een hoofdkwartier voorstelt
			if (symbol == 'h' || symbol == 'H') {
				UnitComponent uc = getUnitComponent(symbol);
				cell->Add(&uc);
			}



		}
	}
	return 0;
}




/*
* Functie die een pointer naar de geïnititaliseerde wereld teruggeeft.
*/
World* World::generateWorld() {
	if (world_file.length() >= 6) {
		/* open() the file */
		ifstream file;
		file.open(world_file);
		if (file.is_open()) {
			World* world;
			string extension = world_file.substr(world_file.length() - 8);
			if (extension == ".world") {
				// geeft nog een error op de ifstream file
				// if (init_world(world, file) != 1) {
					return world;
				//}
				
			}
		}
		file.close();
	}

	return (World*)NULL;
}


/*
* Functie die men oproept om de wereld te initializeren aan de hand van de opgegeven file.
*/
void World::loadLevel() {
	if (generateWorld() == NULL) {
		// loading failed
		// TODO: Fout behandelen
	}
}



