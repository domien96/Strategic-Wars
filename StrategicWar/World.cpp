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

TextureComponent getTextureComponent(char symbol) {
	// TODO: implement switch case get uit texturemap
	return NULL;
}
UnitComponent getUnitComponent(char Symbol) {
	// TODO: implement switch case
	return NULL;
}

/*
* Functie die de wereld initialiseert aan de hand van de data in het gegeven BINAIR bestand.
* Geeft 0 terug indien geslaagd en 1 indien mislukt.
*/
int init_world(World* world, ifstream file) {
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
			//TODO
			Entity* unit = new Entity();
			PositionComponent pc_unit = PositionComponent::PositionComponent(grid, 1);
			
			UnitComponent uc_unit = getUnitComponent(symbol);
			unit->Add(&pc_unit);

			// Voor een hoofdkwartier maken we geen nieuwe entity aan omdat een hoofdkwartier zich toch niet zal 
			// verplaatsen, maar een hoofdkwartier heeft wel een extra unitcomponent nodig, omdat het bezit wordt 
			// door een speler.
			UnitComponent uc = getUnitComponent(symbol);
			cell->Add(&uc);


		}
	}
	return 0;
}




/*
* Functie die een pointer naar de geïnititaliseerde wereld teruggeeft.
*/
World* generateWorld(string world_file) {
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
void loadLevel(std::string world_file) {
	if (generateWorld(world_file) == NULL) {
		// loading failed
		// TODO: Fout behandelen
	}
}



