#include"CloseCombatCondition.h"

CloseCombatCondition::CloseCombatCondition(const bool within_Combat_Range)
{
	setWithinCombatRange(within_Combat_Range);
	name = "Close Combat Condition";
}

CloseCombatCondition::~CloseCombatCondition()
= default;

void CloseCombatCondition::setWithinCombatRange(const bool state)
{
	m_isWithinCombatRange = state;
	data = state;
}

bool CloseCombatCondition::Condition()
{
	return m_isWithinCombatRange;
}
