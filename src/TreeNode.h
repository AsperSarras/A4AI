#pragma once
#ifndef _TREE_NODE_
#define _TREE_NODE_
#include <string>

struct TreeNode
{
	TreeNode() = default;
	virtual ~TreeNode() = default;

	std::string name = "";
	bool data = false;

	TreeNode* Left = nullptr;
	TreeNode* Right = nullptr;
	TreeNode* Parent = nullptr;
	bool isLeaf = false;
};

#endif

