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

	virtual void move(Entity* entity, Grid& from, Grid& to) {
		//TODO
	}

	virtual void Update() {
		EntityStream* es = engine->GetEntityStream();
		World* world = engine->GetContext()->getworld();

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

					Entity* health_bar = new Entity();
					Graphics::Sprite sprite;
					switch (ac->new_hp){
					case 10:
						sprite = Graphics::SPRITE_HEALTH_TEN;
					case 9:
						sprite = Graphics::SPRITE_HEALTH_NINE;
					case 8:
						sprite = Graphics::SPRITE_HEALTH_EIGHT;
					case 7:
						sprite = Graphics::SPRITE_HEALTH_SEVEN;
					case 6:
						sprite = Graphics::SPRITE_HEALTH_SIX;
					case 5:
						sprite = Graphics::SPRITE_HEALTH_FIVE;
					case 4:
						sprite = Graphics::SPRITE_HEALTH_FOUR;
					case 3:
						sprite = Graphics::SPRITE_HEALTH_THREE;
					case 2:
						sprite = Graphics::SPRITE_HEALTH_TWO;
					case 1:
						sprite = Graphics::SPRITE_HEALTH_ONE;
					case 0:
						sprite = Graphics::SPRITE_HEALTH_HALF;
					default:
						sprite = Graphics::SPRITE_HEALTH_TEN;
					}
					health_bar->Add(new TextureComponent(sprite));
					Grid pos = ((PositionComponent*)entity->GetComponent(Component::POSITION))->pos;
					health_bar->Add(new PositionComponent(pos, 4));
					world->GetWorldEntities(4)->push_back(health_bar);
				}
			}
			else {
				PositionComponent* pc = (PositionComponent*)entity->GetComponent(Component::POSITION);
				pc->pos = ac->path->steps[ac->path->steps.size()-1];
				for (size_t i = 0; i < ac->path->steps.size()-1; i++){
					Grid from = ac->path->steps[i];
					Grid to = ac->path->steps[i + 1];
					move(entity, from, to);
				}
				
			}
			entity->Remove(ac);
		}
		

		// TODO 
	};

	virtual Type GetType() { return System::TYPE_ANIMATION; };

private:
};
#endif