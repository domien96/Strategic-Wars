#include "EntityStream.h"
#include "Component.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

EntityStream::EntityStream() {}

EntityStream::~EntityStream() {}

set<Entity*> EntityStream::WithTag(Component::Tag tag)
{
	// TODO: return a set of Entities that match the tag
	return set<Entity*>();
}

set<Entity*> EntityStream::WithTags(set<Component::Tag> tags)
{
	// TODO: return a set of Entities that match all tags
	return set<Entity*>();
}

/* Call this whenever an Entity is added to the Engine */
void EntityStream::EntityAdded(Entity* entity)
{
	// For each tag of entity
	vector<Component::Tag> tags = entity->GetTags();
	for (vector<Component::Tag>::iterator iter = tags.begin(); iter != tags.end(); iter++)
	{
		AddEntityWithTag(entity, *iter);
	}
}

/* Call this whenever an Entity that was already added to the Engine, changes its components */
void EntityStream::EntityChanged(Entity* entity) {
	EntityRemoved(entity);
	EntityAdded(entity);
}

void EntityStream::AddEntityWithTag(Entity* entity, Component::Tag tag)
{
	// TODO: Add this Entity to the set stored under the given tag.
}

/* Call this whenever an Entity is added to the Engine */
void EntityStream::EntityRemoved(Entity* entity)
{
	// For each existing tag
	map<Component::Tag, set<Entity*>>::iterator it = index.begin();
	while (it != index.end()) {
		RemoveEntityWithTag(entity, it->first);
		++it;
	}
}

void EntityStream::RemoveEntityWithTag(Entity* entity, Component::Tag tag)
{
	// TODO: Remove this Entity from the set stored under the given tag.
}