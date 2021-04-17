#pragma once
#ifndef __WAIT_BEHIND_COVER__
#define __WAIT_BEHIND_COVER__
#include "Action.h"

class WaitBehindCover : public Action
{
public:
	WaitBehindCover();
	virtual ~WaitBehindCover();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif 
