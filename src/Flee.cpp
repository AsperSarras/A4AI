#include "Flee.h"
#include <iostream>

Flee::Flee()
{
	m_name = "Flee";
}

Flee::~Flee() {}

void Flee::Execute()
{
	std::cout << "Performing Flee Action..." << std::endl;
}

std::string Flee::getName()
{
	return m_name;
}