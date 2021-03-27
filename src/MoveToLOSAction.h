#pragma once
#ifndef _MOVE_TO_LOS_ACTION_
#define _MOVE_TO_LOS_ACTION_
#include "ActionNode.h"

//Interface(virtual function)
class MoveToLOSAction :public ActionNode
{
public:
	MoveToLOSAction();
	virtual ~MoveToLOSAction();

	virtual void Action() override;
private:

};

#endif
