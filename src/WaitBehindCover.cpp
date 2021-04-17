#include "WaitBehindCover.h"
#include <iostream>

WaitBehindCover::WaitBehindCover()
{
	m_name = "WaitBehindCover";
}

WaitBehindCover::~WaitBehindCover() {}

void WaitBehindCover::Execute()
{
	std::cout << "Performing WaitBehindCover Action..." << std::endl;
}

std::string WaitBehindCover::getName()
{
	return m_name;
}
