#pragma once
#ifndef __PATH_NODE__
#define __PATH_NODE__
#include "NavigationAgent.h"

class PathNode : public NavigationAgent
{
public:
	PathNode();
	~PathNode();

	// DisplayObject Life Cycle Functions
	void draw() override;
	void update() override;
	void clean() override;
private:
};

#endif /* defined (__PATH_NODE__) */
