#include  "NavigationAgent.h"

#include "Util.h"

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

float NavigationAgent::getRotation() const
{
	return m_rotationAngle;
}

void NavigationAgent::setRotation(float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
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

float NavigationAgent::getRangedCombatDistance() const
{
	return m_rangedCombatDistance;
}

bool NavigationAgent::isInRangedCombatDistance() const
{
	return m_isInRangedCombatDistance;
}

glm::vec4 NavigationAgent::getRangedCombatColour() const
{
	return m_RangedCombatColour;
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

void NavigationAgent::setRangedCombatDistance(float distance)
{
	m_rangedCombatDistance = distance;
}

void NavigationAgent::setIsInRangedCombatDistance(bool state)
{
	m_isInRangedCombatDistance = state;
	m_RangedCombatColour = (m_isInRangedCombatDistance) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void NavigationAgent::setRangedCombatColour(glm::vec4 color)
{
	m_RangedCombatColour = color;
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