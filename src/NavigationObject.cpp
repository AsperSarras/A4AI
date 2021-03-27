#include  "NavigationObject.h"

NavigationObject::NavigationObject()
= default;

NavigationObject::~NavigationObject()
= default;

glm::vec2 NavigationObject::getGridPosition() const
{
	return m_gridPosition;
}

void NavigationObject::setGridPosition(const float col, const float row)
{
	m_gridPosition = glm::vec2(col, row);
}

float NavigationObject::getLOSDistance() const
{
	return m_LOSDistance;
}

bool NavigationObject::hasLOS() const
{
	return m_hasLOS;
}

void NavigationObject::setLOSDistance(float distance)
{
	m_LOSDistance = distance;
}

void NavigationObject::setHasLOS(bool state)
{
	m_hasLOS = state;
	m_LOSColor = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}
