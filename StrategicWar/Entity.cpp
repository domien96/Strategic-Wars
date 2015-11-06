#include "Entity.h"

using namespace std;

Entity::Entity()
{
}

Entity::~Entity()
{
}

Entity* Entity::Add(Component * component) {
	// TODO: Add this component to the set stored under the Component's key.
	// TOOD: Return a pointer to this Entity to allow a builder pattern.
	return NULL;
}

Component* Entity::GetComponent(Component::Tag tag)
{
	// TODO: Return the Component associated with the given tag.
	return NULL;
}

Component* Entity::Remove(Component * component)
{
	// TODO: Remove this component entry form the componentmap.
	// TODO: Return a pointer to this component.
	return NULL;
}

vector<Component::Tag> Entity::GetTags()
{
	// TODO: Return a vector with all assigned tags 
	return vector<Component::Tag>();
}

vector<Component*> Entity::GetComponents()
{
	// TODO: Return a vector with all assigend components
	return vector<Component*>();
}