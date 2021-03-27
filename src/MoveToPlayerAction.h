#pragma once
#ifndef _MOVE_TO_PLAYER_ACTION_
#define _MOVE_TO_PLAYER_ACTION_
#include "ActionNode.h"

//Interface(virtual function)
class MoveToPlayerAction :public ActionNode
{
public:
	MoveToPlayerAction();
	virtual ~MoveToPlayerAction();

	virtual void Action() override;
private:

};

#endif
