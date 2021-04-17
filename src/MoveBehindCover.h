#pragma once
#ifndef __MOVE_BEHIND_COVER__
#define __MOVE_BEHIND_COVER__
#include "Action.h"

class MoveBehindCover : public Action
{
public:
	MoveBehindCover();
	virtual ~MoveBehindCover();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif 