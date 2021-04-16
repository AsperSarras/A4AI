#include "MoveToLOS.h"
#include <iostream>

MoveToLOS::MoveToLOS()
{
	m_name = "MoveToLOS";
} // = default;

MoveToLOS::~MoveToLOS(){}

void MoveToLOS::Execute()
{
	std::cout << "Performing MoveToLOS Action..." << std::endl;
}

std::string MoveToLOS::getName()
{
	return m_name;
}
