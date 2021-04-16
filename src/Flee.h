#pragma once
#ifndef __FLEE__
#define __FLEE__
#include "Action.h"

class Flee : public Action
{
public:
	Flee();
	virtual ~Flee();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif /* defined (__FLEE__) */
