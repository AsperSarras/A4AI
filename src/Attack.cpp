#include "Attack.h"
#include <iostream>

Attack::Attack()
{
	m_name = "Attack";
}

Attack::~Attack(){}

void Attack::Execute()
{
	std::cout << "Performing Attack Action..." << std::endl;
}

std::string Attack::getName()
{
	return m_name;
}
