#pragma once
#ifndef __MOVE_TO_LOS__
#define __MOVE_TO_LOS__
#include "Action.h"


class MoveToLOS : public Action
{
public:
	MoveToLOS();
	virtual ~MoveToLOS();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif /* defined (__MOVE_TO_LOS__) */