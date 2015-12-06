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
		/* algoritme voor strategie van Al
		* voor elke unit:
		*	bereken path naar HQ
		*	if (HQ in range)
		*		attack HQ
		*	else
		*		for elke unit van speler
		*			bereken path naar die unit
		*			probeer aan te vallen
		*	if nog AP over
		*		move richting HQ
		*/
		World *world = engine->GetContext()->getworld();
		vector<Entity*> *v = world->GetWorldEntities(1);

		Entity* HQPlayer;
		vector<Entity*> vPlayer; //units van player
		vector<Entity*> vAl; //units van Al

		//units 2 in lijsten verdelen, die van player en die van Al
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

		Pathfinder *finder = new Pathfinder();
		for (vector<Entity*>::iterator it = vAl.begin(); it != vAl.end(); ++it) {
			UnitComponent *uc = static_cast<UnitComponent*>((*it)->GetComponent(Component::UNIT));
			PositionComponent *pc = static_cast<PositionComponent*>((*it)->GetComponent(Component::POSITION));
			//path naar HQ
			PositionComponent *pcHQ = static_cast<PositionComponent*>(HQPlayer->GetComponent(Component::POSITION));
			Path *pHQ = finder->find_path(*world, *uc, pc->pos, pcHQ->pos);
			finder->reset();

			if (pHQ->cost <= uc->range_max && pHQ->cost >= uc->range_min) { //HQ kan aangevallen worden
				//attack HQ
			} else {
				for (vector<Entity*>::iterator it2 = vPlayer.begin(); it2 != vPlayer.end(); ++it2) {
					PositionComponent *pcHuman = static_cast<PositionComponent*>((*it2)->GetComponent(Component::POSITION));
					Path *p = finder->find_path(*world, *uc, pc->pos, pcHuman->pos);
					finder->reset();

					if (p->cost <= uc->range_max && p->cost >= uc->range_min) {
						// attack unit in range
					}
				}
			}
			// naar HQ gaan
		}
	};

	virtual Type GetType() { return System::TYPE_AL; };

private:
};
#endif

