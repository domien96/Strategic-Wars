#ifndef PATH_COMPONENT_H
#define PATH_COMPONENT_H

#include "Component.h"
#include "Pathfinder.h"

class PathComponent : public Component
{
public:
	/* Dit is een eigenschaps-component*/
	PathComponent() {};

	virtual ~PathComponent() {};
	virtual Tag GetTag() { return PATH; };
};

#endif