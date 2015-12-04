#ifndef UNIT_COMPONENT_H
#define UNIT_COMPONENT_H

#include "Component.h"
#include "System.h"

class UnitComponent : public Component
{
public:

	enum UnitType {
		ARCHERY, INFANTRY, FIRE, HQ
	};

	UnitComponent::UnitType type;
	int player;
	int hp, ap, dp, range_min, range_max;

	/*
	 * human player => 0
	 * AI player => 1
	 */
	UnitComponent(UnitComponent::UnitType _type, int _player, int _hp, int _ap, int _dp, int _range_min, int _range_max) :
		type(_type), player(_player), hp(_hp), ap(_ap), dp(_dp), range_min(_range_min), range_max(_range_max) {};
	virtual ~UnitComponent() {};
	virtual Tag GetTag() { return UNIT; };
};

#endif