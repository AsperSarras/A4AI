#pragma once
#ifndef __ATTACK__
#define __ATTACK__
#include "Action.h"

class Attack : public Action
{
public:
	Attack();
	virtual ~Attack();
	std::string m_name;
	virtual void Execute() override;
	std::string getName() override;
private:

};

#endif /* defined (__ATTACK__) */
