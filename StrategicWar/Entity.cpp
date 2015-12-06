#include "Entity.h"

using namespace std;

Entity::Entity()
{
	//components = componentmap(); //niet nodig
}

Entity::~Entity()
{
	for (auto it = components.begin(); it != components.end(); it++) {
		delete (*it).second; // it->second compileert hier niet!
	}
}

Entity* Entity::Add(Component * component) {
	// Add this component to the set stored under the Component's key.
	if (!(component == nullptr))
		components[component->GetTag()] = component;
	// Return a pointer to this Entity to allow a builder pattern.
	return this;
}

Component* Entity::GetComponent(Component::Tag tag)
{
	// Return the Component associated with the given tag.
	return components[tag];
}

Component* Entity::Remove(Component * component)
{
	// Remove this component entry form the componentmap.
	components.erase(component->GetTag());
	// Return a pointer to this component.
	return component;
}

vector<Component::Tag> Entity::GetTags()
{
	vector<Component::Tag> list;
	componentmap::iterator it = components.begin();
	while (it != components.end()) {
		list.push_back(it->first);
		it++;
	}
	// Return a vector with all assigned tags 
 return list;
}

vector<Component*> Entity::GetComponents()
{
	vector<Component*> list;
	componentmap::iterator it = components.begin();
	while (it != components.end()) {
		list.push_back(it->second);
		it++;
	}
	// Return a vector with all assigned components
	return list;
}