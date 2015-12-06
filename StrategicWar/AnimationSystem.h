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

	virtual void move(EntityStream* es, Entity* entity, Grid& from, Grid& to) {
		PositionComponent* pc = (PositionComponent*) entity->GetComponent(Component::POSITION);
		pc->pos = to;
		es->EntityChanged(entity);
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
					UnitComponent* uc = (UnitComponent*)entity->GetComponent(Component::UNIT);
					uc->ap = ac->new_ap;
					engine->UpdateEntity(entity);
				}
				else {
					UnitComponent* uc = (UnitComponent*) entity->GetComponent(Component::UNIT);
					uc->hp = ac->new_hp;

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
						sprite = Graphics::SPRITE_NONE;
					default:
						sprite = Graphics::SPRITE_HEALTH_TEN;
					}

					Grid pos = ((PositionComponent*)entity->GetComponent(Component::POSITION))->pos;

					if (world->getWorldEntity(pos.row, pos.col, 4) == NULL) {
						Entity* health_bar = new Entity();
						health_bar->Add(new TextureComponent(sprite));
						health_bar->Add(new PositionComponent(pos, 4));
						world->GetWorldEntities(4)->push_back(health_bar);
						engine->AddEntity(health_bar);
					} else {
						world->getWorldEntity(pos.row, pos.col, 4)->Add(new TextureComponent(sprite));
						engine->UpdateEntity(world->getWorldEntity(pos.row, pos.col, 4));
					}

					//Remove if dead
					if (uc->hp <= 0) {
						entity->Remove(entity->GetComponent(Component::POSITION));
						entity->Remove(entity->GetComponent(Component::TEXTURE));
						entity->Remove(entity->GetComponent(Component::UNIT));
						engine->RemoveEntity(entity);
					}
				}
			}
			else {
				//PositionComponent* pc = (PositionComponent*)entity->GetComponent(Component::POSITION);
				//pc->pos = ac->path->steps[ac->path->steps.size()-1];
				UnitComponent* uc = (UnitComponent*)entity->GetComponent(Component::UNIT);
				uc->ap = ac->new_ap;
				es->EntityChanged(entity);

				for (size_t i = 0; i < ac->path->steps.size()-1; i++){
					Grid from = ac->path->steps[i];
					Grid to = ac->path->steps[i + 1];
					move(es, entity, from, to);
				}
				
			}
			entity->Remove(ac);
		}
	};

	virtual Type GetType() { return System::TYPE_ANIMATION; };

private:
};
#endif