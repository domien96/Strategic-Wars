#ifndef ENTITY_STREAM_H
#define ENTITY_STREAM_H

#include <map>
#include <string>
#include <set>
#include "Entity.h"

class EntityStream
{
public:
	EntityStream();
	~EntityStream();

	/* Returns a set of Entity* that contain the given tag */
	std::set<Entity*> WithTag(Component::Tag tag);

	/* Returns a set of Entity* that contains at least all of the given tags */
	std::set<Entity*> WithTags(std::set<Component::Tag> tags);

	/* Hook that gets called by the engine everytime a new Entity is added to the engine */
	void EntityAdded(Entity* entity);

	/* Hook that gets called by the engine everytime a new Entity, already added to the engine, got its Components changed */
	void EntityChanged(Entity* entity);

	/* Hook that gets called by the engine everytime an added Entity is removed from the engine */
	void EntityRemoved(Entity* entity);

private:
	// Fields
	std::map<Component::Tag, std::set<Entity*>> index;

	// Methods
	void AddEntityWithTag(Entity* entity, Component::Tag tag);
	void RemoveEntityWithTag(Entity* entity, Component::Tag tag);
	
};

#endif