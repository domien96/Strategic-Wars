#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Component.h"
#include "Pathfinder.h"

class AnimationComponent : public Component
{
public:

	/* True if the animation is an attack, false if it is a movement */
	bool is_attack;

	/* Path to follow, only if the animation is a movement */
	Path* path;
	
	/* True if this unit is the attacker, false if it is being attacked; only if the animation is an attack */
	bool is_attacker;

	/* New hp, only if the animation is an attack*/
	int new_hp;

	/* new ap, always relevant */
	int new_ap;


	/* Call if animation is a movement*/
	AnimationComponent(Path& _path, int _new_ap) : is_attack(false), path(&_path), is_attacker(false), new_hp(0), new_ap(_new_ap) {};
	/* Call if animation is an attack*/
	AnimationComponent(int _new_hp, bool _is_attacker, int _new_ap) :
		is_attack(true), path(NULL), is_attacker(_is_attacker), new_hp(_new_hp), new_ap(_new_ap) {};

	virtual ~AnimationComponent() {};
	virtual Tag GetTag() { return ANIMATION; };
};

#endif