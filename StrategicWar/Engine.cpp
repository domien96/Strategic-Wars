#include "Engine.h"
#include "System.h"
#include "EntityStream.h"
#include "AllegroLib.h"

using namespace std;

void Engine::AddEntity(Entity* entity)
{
	// TODO: Add entity to engine
	// TODO: Warn EntityStream
}

void Engine::UpdateEntity(Entity* entity)
{
	// TODO: Warn EntityStream
}

void Engine::AddSystem(System* system)
{
	// TODO: Add System to engine
	// TODO: Set engine pointer of system to this engine
}

void Engine::RemoveEntity(Entity* entity)
{
	// TODO: Remove entity from engine
	// TODO: Warn EntityStream
}

void Engine::RemoveSystem(System* system)
{
	// TODO: Remove System from engine
	// TODO: Reset engine pointer of system to NULL
}

vector<Entity*> Engine::GetEntities()
{
	// TODO: Return all assigned entities.
	return vector<Entity*>();
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