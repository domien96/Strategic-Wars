#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vector2.h"
#include "Grid.h"

class Component
{
public:
	enum Tag {
		TEXTURE, POSITION, UNIT, ANIMATION, PATH
	};

	Component() {};
	virtual ~Component() {};
	virtual Tag GetTag() = 0;

};

#endif