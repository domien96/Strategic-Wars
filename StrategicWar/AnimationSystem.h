#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"

class AnimationSystem : public System
{
public:
	AnimationSystem() :
		System() {};
	~AnimationSystem() {};

protected:
	virtual void Update() {
		// TODO 
	};

	virtual Type GetType() { return System::TYPE_ANIMATION; };

private:
};
#endif