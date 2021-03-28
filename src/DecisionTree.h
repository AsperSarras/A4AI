#pragma once
#ifndef _DECISION_TREE_
#define _DECISION_TREE_
#include "NavigationAgent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

class DecisionTree
{
public:
	//Constructor Destructor
	DecisionTree();
	~DecisionTree();

	//Geters and setters
	NavigationAgent* getAgent() const;
	void setAgent(NavigationAgent* agent);

	//Convienece Functions
	TreeNode* addNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void DisplayTree();
	void Update();

	std::string MakeDecision(); //in order traversal
private:
	NavigationAgent* m_agent{};

	LOSCondition* m_LOSNode{};
	RadiusCondition* m_RadiusNode{};
	CloseCombatCondition* m_CloseCombatNode{};
	
	void m_buildTree();
	
	std::vector<TreeNode*>m_treeNodesList;
};

#endif
