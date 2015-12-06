#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Engine.h"

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
	};

	virtual Type GetType() { return System::TYPE_AL; };

private:
};
#endif

