#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <map>
#include "Context.h"
#include "Entity.h"
#include "System.h"
#include "EntityStream.h"
#include "World.h"


class Engine
{
public:
	Engine(Context* _context) : context(_context) {};
	~Engine() {};

	World *world;
	void SetWorld(World *w);

	/* Adds an Entity to the Engine, warning the EntityStream. */
	void AddEntity(Entity* entity);

	/* Update an Entity added to the Engine, warning the EntityStream */
	void UpdateEntity(Entity* entity);

	/* Adds a System to the Engine */
	void AddSystem(System* system);

	/* Removes an Entity from the Engine, warning the EntityStream */
	void RemoveEntity(Entity* entity);

	/* Removes a System from the Engine */
	void RemoveSystem(System* system);

	/* Returns all Entities currently attached to the Engine */
	std::vector<Entity*> GetEntities();

	/* Calls Updates on all added Systems in order */
	void Update();

	/* Returns a pointer to the Context object */
	Context* GetContext();

	/* Returns a System associated with the given type */
	System* GetSystem(System::Type type);

	/* Returns a pointer to the EntityStream object of this Entity */
	EntityStream * GetEntityStream();

private:
	/* Warns the EntityStream that an entity was added */
	void EntityAdded(Entity* entity);

	/* Warns the EntityStream that an entity was changed */
	void EntityChanged(Entity* entity);

	/* Warns the EntityStream that an entity was removed */
	void EntityRemoved(Entity* entity);

	std::vector<Entity*> entities;
	std::vector<System*> systems;
	std::map<System::Type, System*> index;

	Context *context;
	double time_now, time_prev;
	EntityStream es;
};

#endif