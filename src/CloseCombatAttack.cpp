#include "CloseCombatAttack.h"
#include <iostream>

CloseCombatAttack::CloseCombatAttack()
{
	m_name = "CloseCombatAttack";
}

CloseCombatAttack::~CloseCombatAttack(){}

void CloseCombatAttack::Execute()
{
	std::cout << "Performing CloseCombatAttack Action..." << std::endl;
}

std::string CloseCombatAttack::getName()
{
	return m_name;
}
