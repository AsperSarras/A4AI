#include "RangedAttack.h"
#include <iostream>

RangedAttack::RangedAttack()
{
	m_name = "RangedAttack";
}

RangedAttack::~RangedAttack() {}

void RangedAttack::Execute()
{
	std::cout << "Performing RangedAttack Action..." << std::endl;
}

std::string RangedAttack::getName()
{
	return m_name;
}