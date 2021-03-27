#pragma once
#ifndef _ATTACK_ACTION_
#define _ATTACK_ACTION_
#include "ActionNode.h"

//Interface(virtual function)
class AttackAction :public ActionNode
{
public:
	AttackAction();
	virtual ~AttackAction();

	virtual void Action() override;
private:

};

#endif