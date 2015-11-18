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

void Engine::AddSystem(System* system)
{
	// TODO: Add System to engine
	// TODO: Set engine pointer of system to this engine
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

void Engine::RemoveSystem(System* system)
{
	// TODO: Remove System from engine
	// TODO: Reset engine pointer of system to NULL
}

/* Returns all Entities currently attached to the Engine. */
vector<Entity*> Engine::GetEntities()
{
	return entities;
}

void Engine::Update()
{
	// TODO: Call Update() on all assigned systems in a fixed order.
}


void Engine::EntityAdded(Entity* entity)
{
	// TODO: Warn EntityStream
}

void Engine::EntityChanged(Entity* entity)
{
	// TODO: Warn EntityStream
}

void Engine::EntityRemoved(Entity* entity)
{
	// TODO: Warn EntityStream
}

Context* Engine::GetContext()
{
	// TODO: Return context pointer
	return NULL;
}

System* Engine::GetSystem(System::Type type)
{
	// TODO: Return requested System
	return NULL;
}

EntityStream* Engine::GetEntityStream()
{
	// TODO: Return EntityStream
	return NULL;
}