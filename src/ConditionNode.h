#pragma once
#ifndef _CONDITION_NODE_
#define _CONDITION_NODE_
#include "TreeNode.h"

//Interface(virtual function)
class ConditionNode :public TreeNode
{
public:
	ConditionNode() { isLeaf = false; }
	virtual ~ConditionNode() = default;

	virtual bool Condition() = 0;
};


#endif