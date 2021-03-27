#include "Agents.h"

#include "Util.h"

Agents::Agents()
{
}

Agents::~Agents()
= default;

glm::vec2 Agents::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agents::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agents::getLOSDistance() const
{
	return m_LOSDistance;
}

bool Agents::hasLOS() const
{
	return m_hasLOS;
}

float Agents::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agents::getLOSColour() const
{
	return m_LOSColor;
}

void Agents::setTargetPosition(const glm::vec2 new_Position)
{
	m_targetPosition = new_Position;
}

void Agents::setCurrentDirection(const glm::vec2 new_Direction)
{
	m_currentDirection = new_Direction;
}

void Agents::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agents::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColor = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agents::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agents::setLOSColor(const glm::vec4 color)
{
	m_LOSColor = color;
}

void Agents::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}
