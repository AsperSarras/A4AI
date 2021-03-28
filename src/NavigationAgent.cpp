#include  "NavigationAgent.h"

NavigationAgent::NavigationAgent()
= default;

NavigationAgent::~NavigationAgent()
= default;

glm::vec2 NavigationAgent::getGridPosition() const
{
	return m_gridPosition;
}

void NavigationAgent::setGridPosition(const float col, const float row)
{
	m_gridPosition = glm::vec2(col, row);
}

float NavigationAgent::getLOSDistance() const
{
	return m_LOSDistance;
}

glm::vec4 NavigationAgent::getLOSColour() const
{
	return m_LOSColor;
}

float NavigationAgent::getCloseCombatDistance() const
{
	return m_closeCombatDistance;
}

bool NavigationAgent::isInCloseCombatDistance() const
{
	return m_isInCloseCombatDistance;
}

glm::vec4 NavigationAgent::getCloseCombatColour() const
{
	return m_CloseCombatColour;
}

void NavigationAgent::setCloseCombatDistance(float distance)
{
	m_closeCombatDistance = distance;
}

void NavigationAgent::setIsInCloseCombatDistance(bool state)
{
	m_isInCloseCombatDistance = state;
	m_CloseCombatColour = (m_isInCloseCombatDistance) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void NavigationAgent::setCloseCombatColour(glm::vec4 color)
{
	m_CloseCombatColour = color;
}

bool NavigationAgent::hasLOS() const
{
	return m_hasLOS;
}

void NavigationAgent::setLOSDistance(float distance)
{
	m_LOSDistance = distance;
}

void NavigationAgent::setHasLOS(bool state)
{
	m_hasLOS = state;
	m_LOSColor = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void NavigationAgent::setLOSColor(const glm::vec4 color)
{
	m_LOSColor = color;
}