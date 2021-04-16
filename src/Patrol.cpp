#include "Patrol.h"
#include <iostream>

Patrol::Patrol()
{
	m_name = "Patrol";
}

Patrol::~Patrol(){}

void Patrol::Execute()
{
	std::cout << "Performing Patrol Action..." << std::endl;
}

std::string Patrol::getName()
{
	return m_name;
}
