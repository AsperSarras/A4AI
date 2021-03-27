#pragma once
#ifndef _CLOSE_COMBAT_CONDITION_
#define _CLOSE_COMBAT_CONDITION_
#include "ConditionNode.h"

class CloseCombatCondition :public ConditionNode
{
public:
	CloseCombatCondition(bool within_Combat_Range = false);
	virtual ~CloseCombatCondition();

	//Getters Setters
	void setWithinCombatRange(bool state);

	virtual bool Condition() override;
private:
	bool m_isWithinCombatRange;
};

#endif