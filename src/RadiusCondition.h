#pragma once
#ifndef _RADIUS_CONDITION_
#define _RADIUS_CONDITION_
#include "ConditionNode.h"

class RadiusCondition :public ConditionNode
{
public:
	RadiusCondition(bool within_radius = false);
	virtual ~RadiusCondition();

	//Getters Setters
	void setIsWithinRadius(bool state);

	virtual bool Condition() override;
private:
	bool m_isWithinRadius;
};

#endif
