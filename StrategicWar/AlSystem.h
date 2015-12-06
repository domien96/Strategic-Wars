#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "World.h"
#include "Entity.h"
#include "Pathfinder.h"
#include "PositionComponent.h"

#include <vector>

using namespace std;

class AlSystem : public System
{
public:
	AlSystem() :
		System() {};
	~AlSystem() {};

protected:
	virtual void Update() {
		/* eenvoudige strategie
		* voor elke unit:
		*	als je iets kan aanvallen -> val aan
		*	anders bereken path naar headquarter -> move zo ver mogelijk
		*/
		World *world = engine->GetContext()->getworld();
		vector<Entity*> *v = world->GetWorldEntities(1);

		Entity* HQPlayer;
		vector<Entity*> vPlayer; //units van player
		vector<Entity*> vAl; //units van Al

		for (vector<Entity*>::iterator it = v->begin(); it != v->end(); ++it) {
			if ((*it) != nullptr ) {
				UnitComponent *uc = static_cast<UnitComponent*>((*it)->GetComponent(Component::UNIT));
				if (uc != nullptr && UnitComponent::UnitType::HQ != uc->type) { //alle units
					if (uc->player == 0) { // player
						vPlayer.push_back(*it);
					}
					else { // Al
						vAl.push_back(*it);
					}
				} else if (uc != nullptr && UnitComponent::UnitType::HQ == uc->type && uc->player == 0) {
					HQPlayer = (*it);
				}
			}
		}

		for (vector<Entity*>::iterator it = vAl.begin(); it != vAl.end(); ++it) {
			//kijken of er iets kan aangevallen worden adhv pad
			Pathfinder *finder = new Pathfinder();
			UnitComponent *uc = static_cast<UnitComponent*>((*it)->GetComponent(Component::UNIT));
			PositionComponent *pc = static_cast<PositionComponent*>((*it)->GetComponent(Component::POSITION));
			//eerst nog een controle of de hq zelf kan aangevallen worden
			for (vector<Entity*>::iterator it2 = vPlayer.begin(); it2 != vPlayer.end(); ++it2) {
				PositionComponent *pcHuman = static_cast<PositionComponent*>((*it2)->GetComponent(Component::POSITION));

				Path *p = finder->find_path(*world, *uc, pc->pos, pcHuman->pos);
				//controle of p kort genoeg is
				//aanval
			}
			//path naar hq
		}
	};

	virtual Type GetType() { return System::TYPE_AL; };

private:
};
#endif

