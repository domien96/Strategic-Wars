#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Component.h"

class AnimationComponent : public Component
{
public:
	//Grid pos;
	//int z; //Depth in the layer system. The lowest component has depth 0.

	//AnimationComponent(Grid _pos, int _z) : pos(_pos), z(_z) {};
	virtual ~AnimationComponent() {};
	virtual Tag GetTag() { return ANIMATION; };
};

#endif