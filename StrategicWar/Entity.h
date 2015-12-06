#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <vector>
#include "Component.h"

typedef std::map<Component::Tag, Component*> componentmap;

class Entity
{
public:
	Entity() : components(new componentmap()) {};
	~Entity();

	/* Add a component to the entity, nothing happens if component is a nullpointer or points to a non-accessable location*/
	Entity* Add(Component * component);

	/* Return the Component associated with the given tag */
	Component* GetComponent(Component::Tag tag);

	/* Remove a component from the entity */
	Component* Remove(Component * component);

	/* Get all assigned Tags of this Entity */
	std::vector<Component::Tag> GetTags();

	/* Get all assigned Components of this Entity */
	std::vector<Component*> GetComponents();

private:
	componentmap* components;
};

#endif