#include "MoveBehindCover.h"
#include <iostream>

MoveBehindCover::MoveBehindCover()
{
	m_name = "MoveBehindCover";
}

MoveBehindCover::~MoveBehindCover() {}

void MoveBehindCover::Execute()
{
	std::cout << "Performing MoveBehindCover Action..." << std::endl;
}

std::string MoveBehindCover::getName()
{
	return m_name;
}
