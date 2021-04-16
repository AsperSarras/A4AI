#pragma once
#ifndef __ACTION__
#define __ACTION__
#include <string>

// Action Interface
class Action
{
public:
	Action() = default;
	virtual ~Action() = default;

	virtual  std::string getName() = 0;
	virtual void Execute() = 0;
};
#endif /* defined (__ACTION__) */