#include "MoveToPlayer.h"
#include <iostream>

MoveToPlayer::MoveToPlayer()
{
	m_name = "MoveToPlayer";
}

MoveToPlayer::~MoveToPlayer(){}

void MoveToPlayer::Execute()
{
	std::cout << "Performing MoveToPlayer Action..." << std::endl;
}

std::string MoveToPlayer::getName()
{
	return m_name;
}
