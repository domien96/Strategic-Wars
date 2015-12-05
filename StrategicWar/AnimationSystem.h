#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "UnitComponent.h"
#include "AnimationComponent.h"
#include "PositionComponent.h"
#include "EntityStream.h"
#include "Engine.h"

using namespace std;

class AnimationSystem : public System
{
public:
	AnimationSystem() :
		System() {};
	~AnimationSystem() {};

protected:
	virtual void Update() {
		EntityStream* es = engine->GetEntityStream();

		set<Entity*> entity_set = es->WithTag(Component::ANIMATION);
		for (set<Entity*>::iterator i = entity_set.begin(); i!= entity_set.end(); i++) {
			Entity* entity = *i;
			AnimationComponent* ac = (AnimationComponent*) entity->GetComponent(Component::ANIMATION);
			if (ac->is_attack) {
				if (ac->is_attacker) {
					// TODO
				}
				else {
					UnitComponent* uc = (UnitComponent*) entity->GetComponent(Component::UNIT);
					uc->hp = ac->new_hp;
				}
			}
			else {
				PositionComponent* pc = (PositionComponent*)entity->GetComponent(Component::POSITION);
				pc->pos = *ac->new_pos;
			}
			entity->Remove(ac);
		}
		

		// TODO 
	};

	virtual Type GetType() { return System::TYPE_ANIMATION; };

private:
};
#endif