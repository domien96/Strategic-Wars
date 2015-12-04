#include "../StrategicWar/Engine.h"
#include "../StrategicWar/Entity.h"
#include "../StrategicWar/EntityStream.h"
#include "../StrategicWar/Game.h"
#include "../StrategicWar/Pathfinder.h"
#include "../StrategicWar/MouseSystem.h"
#include "../StrategicWar/RenderSystem.h"
#include "../StrategicWar/AnimationSystem.h"
#include <assert.h>


void test_engine_add_entity() {
	Vector2 v;
	Engine e(new Context(v));
	assert(e.GetEntities().size() == 0);
	Entity* e1 = new Entity();
	e.AddEntity(e1);
	assert(e.GetEntities().size() == 1);
	Entity* e2 = new Entity();
	e.AddEntity(e2);
	assert(e.GetEntities().size() == 2);
	delete e1, e2;
}

void test_add_system(){
	Vector2 v;
	Engine e(new Context(v));
	assert(e.GetSystem(System::TYPE_MOUSE) == NULL);
	assert(e.GetSystem(System::TYPE_RENDER) == NULL);
	assert(e.GetSystem(System::TYPE_ANIMATION) == NULL);

	System* ms = new MouseSystem();
	e.AddSystem(ms);
	assert(e.GetSystem(System::TYPE_MOUSE) == ms);

	System* rs = new RenderSystem();
	e.AddSystem(ms);
	assert(e.GetSystem(System::TYPE_RENDER) == rs);

	System* as = new RenderSystem();
	e.AddSystem(as);
	assert(e.GetSystem(System::TYPE_ANIMATION) == as);

	delete ms,rs,as;
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