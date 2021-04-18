#pragma once
#ifndef __CLOSE_COMBAT_ATTACK__
#define __CLOSE_COMBAT_ATTACK__
#include "Action.h"

class CloseCombatAttack : public Action
{
public:
	CloseCombatAttack();
	virtual ~CloseCombatAttack();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif 
