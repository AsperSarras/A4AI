#include "AttackAction.h"
#include <iostream>

#include "PatrolAction.h"

AttackAction::AttackAction()
{
	name = "Attack Action";
}

AttackAction::~AttackAction()
= default;

void AttackAction::Action()
{
	std::cout << "Performing Attack Action" << std::endl;
}