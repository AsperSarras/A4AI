#pragma once
#ifndef _ACTION_NODE_
#define _ACTION_NODE_
#include "TreeNode.h"

//Interface(virtual function)
class ActionNode :public TreeNode
{
public:
	ActionNode() { isLeaf = true; }
	virtual ~ActionNode() = default;

	virtual void Action() = 0;
};

#endif

