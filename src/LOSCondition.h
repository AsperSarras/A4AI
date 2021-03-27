#pragma once
#ifndef _LOS_CONDITION_
#define _LOS_CONDITION_
#include "ConditionNode.h"

class LOSCondition :public ConditionNode
{
public:
	LOSCondition(bool LOS=false);
	virtual ~LOSCondition();

	//Getters Setters
	void setLOS(bool state);

	virtual bool Condition() override;
private:
	bool m_hasLOS;
};

#endif