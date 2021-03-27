#pragma once
#ifndef _PATROL_ACTION_
#define _PATROL_ACTION_
#include "ActionNode.h"

//Interface(virtual function)
class PatrolAction :public ActionNode
{
public:
	PatrolAction();
	virtual ~PatrolAction();

	virtual void Action() override;
private:
	
};

#endif
