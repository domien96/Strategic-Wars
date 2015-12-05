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
		EntityStream* es = engine->GetEntityStream();
		vector<Entity*> entities = engine->GetEntities();
		Context* context = engine->GetContext();

		//vector<Entity*> en = es->WithTag(Component::UNIT);
		

		// TODO 
	};

	virtual Type GetType() { return System::TYPE_ANIMATION; };

private:
};
#endif