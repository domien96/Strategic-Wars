#include "World.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
* Functie die de wereld initialiseert aan de hand van de data in het gegeven bestand.
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
			// TODO: initialize every cell + store these cells
			// Cell = row, col, celltype, owner
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
			string extension = world_file.substr(world_file.length() - 5);
			if (extension == ".wld") {
				//init_world_binary();
				//return world;
			}
			extension = world_file.substr(world_file.length() - 8);
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



