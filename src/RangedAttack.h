#pragma once
#ifndef __RANGED_COMBAT_ATTACK__
#define __RANGED_COMBAT_ATTACK__
#include "Action.h"

class RangedAttack : public Action
{
public:
	RangedAttack();
	virtual ~RangedAttack();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif 
