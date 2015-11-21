#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "Component.h"

class PositionComponent : public Component
{
public:
	Grid pos;
	int z; //Depth in the layer system

	PositionComponent(Grid _pos, int _z) : pos(_pos), z(_z) {};
	virtual ~PositionComponent() {};
	virtual Tag GetTag() { return POSITION; };
};

#endif