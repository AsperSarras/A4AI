#pragma once
#ifndef __PATROL__
#define __PATROL__
#include "Action.h"
#include <string>

class Patrol : public Action
{
public:
	Patrol();
	virtual ~Patrol();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif /* defined (__PATROL__) */