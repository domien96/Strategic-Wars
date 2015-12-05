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
	
	/* New hp, only if the animation is an attack*/
	int new_hp;


	/* Call if animation is a movement*/
	AnimationComponent(Grid& _new_pos) : is_attack(false), new_pos(&_new_pos), new_hp(0) {};
	/* Call if animation is an attack*/
	AnimationComponent(int _new_hp) : is_attack(true), new_pos(NULL), new_hp(_new_hp) {};

	virtual ~AnimationComponent() {};
	virtual Tag GetTag() { return ANIMATION; };
};

#endif