#include "DecisionTree.h"

#include <iostream>



#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "PatrolAction.h"

DecisionTree::DecisionTree()
{
	m_buildTree();
}

DecisionTree::~DecisionTree()
= default;

NavigationAgent* DecisionTree::getAgent() const
{
	return m_agent;
}

void DecisionTree::setAgent(NavigationAgent* agent)
{
	m_agent = agent;
}

TreeNode* DecisionTree::addNode(TreeNode* parent, TreeNode* child_node, const TreeNodeType type)
{
	switch (type)
	{
	case LEFT_TREE_NODE:
		parent->Left = child_node; 
		break;
	case RIGHT_TREE_NODE:
		parent->Right = child_node;
		break;
	}
	child_node->Parent = parent;//for backwards traversal
	return child_node;
}

void DecisionTree::DisplayTree()
{
	for (auto* node : m_treeNodesList)
	{
		std::cout << node->name << std::endl;
	}
}

void DecisionTree::Update()
{
	m_LOSNode->setLOS(m_agent->hasLOS());
}

//in order traversal
std::string DecisionTree::MakeDecision()
{
	Update();
	
	auto currentNode = m_treeNodesList[0];//root node
	while(!currentNode->isLeaf)
	{
		currentNode = (currentNode->data) ? (currentNode->Right) : (currentNode->Left);
	}
	return currentNode->name;
}

void DecisionTree::m_buildTree()
{
	//add the root node
	m_LOSNode = new LOSCondition();
	m_treeNodesList.push_back(m_LOSNode);//Node 0

	m_RadiusNode = new RadiusCondition();
	addNode(m_LOSNode, m_RadiusNode,LEFT_TREE_NODE);
	m_treeNodesList.push_back(m_RadiusNode); //Node1

	m_CloseCombatNode = new CloseCombatCondition();
	addNode(m_LOSNode, m_CloseCombatNode, RIGHT_TREE_NODE);
	m_treeNodesList.push_back(m_CloseCombatNode); //Node2

	TreeNode* PatrolNode = addNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
	m_treeNodesList.push_back(PatrolNode); //Node3

	TreeNode* MoveToLOSNode = addNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	m_treeNodesList.push_back(MoveToLOSNode); //Node4

	TreeNode* MoveToPlayerNode = addNode(m_CloseCombatNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
	m_treeNodesList.push_back(MoveToPlayerNode); //Node5

	TreeNode* AttackNode = addNode(m_CloseCombatNode, new AttackAction(), RIGHT_TREE_NODE);
	m_treeNodesList.push_back(AttackNode); //Node6
	
}
