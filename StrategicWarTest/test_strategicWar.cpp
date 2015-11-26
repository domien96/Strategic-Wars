#include "../StrategicWar/Engine.h"
#include "../StrategicWar/Entity.h"
#include "../StrategicWar/EntityStream.h"
#include "../StrategicWar/Game.h"
#include "../StrategicWar/Pathfinder.h"
#include <assert.h>


void test_engine_add_entity() {
	Vector2 v;
	Engine e(new Context(v));
	e.AddEntity(new Entity());
	assert(e.GetEntities().size()==1);
}


void runTests() {
	// DOE TOPDOWN TESTEN, WANT HET BESPAART VEEL WERK.
	// Voeg methodes toe en schrijf daarin je testen

	// Testen voor entity

	// Testen voor Engine



	// Testen voor ?
	// Testen voor ?
	// enz...
}


int main() {
	runTests();
}