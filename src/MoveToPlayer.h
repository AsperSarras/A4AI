#pragma once
#ifndef __MOVE_TO_PLAYER__
#define __MOVE_TO_PLAYER__
#include "Action.h"


class MoveToPlayer : public Action
{
public:
	MoveToPlayer();
	virtual ~MoveToPlayer();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif /* defined (__MOVE_TO_PLAYER_ACTION__) */