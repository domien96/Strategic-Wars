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
	return index[tag];
}

set<Entity*> EntityStream::WithTags(set<Component::Tag> tags)
{
	set<Entity*> result_set;

	if (tags.empty()) {
		for (map<Component::Tag, std::set<Entity*>>::iterator it = index.begin(); it != index.end(); it++) {
			set<Entity*> s = it->second;
			result_set.insert(s.begin(), s.end());
		}
		return result_set;
	}

	set<Entity*> begin_set = index[*(tags.begin())];

	for (set<Entity*>::iterator it = begin_set.begin(); it != begin_set.end(); it++) {
		bool add = true;
		for (set<Component::Tag>::iterator it2 = ++tags.begin(); it2 != tags.end(); it2++) {
			vector<Component::Tag> t = (**it).GetTags();
			if(find(t.begin(), t.end(), *it2) == t.end()) {
				add = false;
			}
		}
		if (add) {
			result_set.insert(*it);
		}
	}
	return result_set;
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
	if (index.find(tag) == index.end()) {
		index[tag] = {entity};
	} else {
		index[tag].insert(entity);
	}
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
	if (index.find(tag) != index.end()) {
		index[tag].erase(entity);
	}
}