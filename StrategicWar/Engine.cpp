#include "Engine.h"
#include "System.h"
#include "EntityStream.h"
#include "AllegroLib.h"

using namespace std;

/* If the Enitity is not yet attached to the Engine, then it will be added and the EntityStream will be warned. */
void Engine::AddEntity(Entity* entity)
{
	std::vector<Entity*>::iterator it;
	it = find(entities.begin(), entities.end(), entity);
	if (it == entities.end()) {
		entities.push_back(entity);
		EntityAdded(entity);
	}
}

/* Update an Entity added to the Engine, warning the EntityStream. */
void Engine::UpdateEntity(Entity* entity)
{
	EntityChanged(entity);
}

/* If the System is not yet attached to the Engine, then it will be added. We will also set the engine pointer of the System to this Engine. */
void Engine::AddSystem(System* system)
{
	std::vector<System*>::iterator it;
	it = find(systems.begin(), systems.end(), system);
	if (it == systems.end()) {
		systems.push_back(system);
	}
	system->SetEngine(this);
}

/* In case the given Entity is attached to the Engine, it is removed and the EntityStream is warned. */
void Engine::RemoveEntity(Entity* entity)
{
	std::vector<Entity*>::iterator it;
	it = find(entities.begin(), entities.end(), entity);
	if (it != entities.end()) {
		entities.erase(it);
		EntityRemoved(entity);
	}
}

/* In case the given System is attached to the Engine, it is removed. We will also reset the engine pointer of the System to NULL. */
void Engine::RemoveSystem(System* system)
{
	std::vector<System*>::iterator it;
	it = find(systems.begin(), systems.end(), system);
	if (it != systems.end()) {
		systems.erase(it);
	}
	system->SetEngine(NULL);
}

/* Returns all Entities currently attached to the Engine. */
vector<Entity*> Engine::GetEntities()
{
	return entities;
}


/* Calls Updates on all added Systems in order */
void Engine::Update()
{
	std::vector<System*>::iterator it;
	for (it = systems.begin(); it < systems.end(); ++it) {
		(*it)->Update();
	}
}

/* Warns the EntityStream that an entity was added */
void Engine::EntityAdded(Entity* entity)
{
	GetEntityStream()->EntityAdded(entity);
}

/* Warns the EntityStream that an entity was changed */
void Engine::EntityChanged(Entity* entity)
{
	GetEntityStream()->EntityChanged(entity);
}

/* Warns the EntityStream that an entity was removed */
void Engine::EntityRemoved(Entity* entity)
{
	GetEntityStream()->EntityRemoved(entity);
}

/* Returns a pointer to the Context object */
Context* Engine::GetContext()
{
	return context;
}

System* Engine::GetSystem(System::Type type)
{
	// TODO: Return requested System
	return NULL;
}

/* Returns a pointer to the EntityStream object of this Entity */
EntityStream* Engine::GetEntityStream()
{
	return &es;
}