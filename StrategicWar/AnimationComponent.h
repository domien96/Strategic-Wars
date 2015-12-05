#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Component.h"

class AnimationComponent : public Component
{
public:

	/* True if the animation is an attack, false if it is a movement */
	bool is_attack;

	/* New position, only if the animation is a movement */
	Grid* new_pos;
	
	/* True if this unit is the attacker, false if it is being attacked; only if the animation is an attack */
	bool is_attacker;

	/* New hp, only if the animation is an attack*/
	int new_hp;


	/* Call if animation is a movement*/
	AnimationComponent(Grid& _new_pos) : is_attack(false), new_pos(&_new_pos), is_attacker(false), new_hp(0) {};
	/* Call if animation is an attack*/
	AnimationComponent(int _new_hp, bool _is_attacker) :
		is_attack(true), new_pos(NULL), is_attacker(_is_attacker), new_hp(_new_hp) {};

	virtual ~AnimationComponent() {};
	virtual Tag GetTag() { return ANIMATION; };
};

#endif