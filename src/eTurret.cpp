#include "eTurret.h"

#include "Game.h"
#include "Util.h"

eTurret::eTurret()
{
	TextureManager::Instance()->load("../Assets/sprites/Health.png", "df");

	auto size = TextureManager::Instance()->getTextureSize("df");
	setWidth(20);
	setHeight(size.y/*100*/);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ETURRET);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(0.0f);
	setTurnRate(2.0f);

	setLOSDistance(250.0f);// 5 pixel per frame * 80 feet
	m_LOSColor = glm::vec4(1, 0, 0, 1);//red
	
}

eTurret::~eTurret()
= default;

void eTurret::draw()
{
	TextureManager::Instance()->draw("hp", 
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	/*Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f) );*/

	//draw LOS
	//Util::DrawLine(getTransform()->position, getTransform()->position + getOrientation() * getLOSDistance(), m_LOSColor);

}

void eTurret::update()
{
	//m_Move();
}

void eTurret::clean()
{
}

void eTurret::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void eTurret::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

//glm::vec2 eTurret::getOrientation() const
//{
//	return m_orientation;
//}

float eTurret::getTurnRate() const
{
	return m_turnRate;
}

void eTurret::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float eTurret::getAccelerationRate() const
{
	return m_accelerationRate;
}

void eTurret::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

//float eTurret::getLOSDistance() const
//{
//	return m_LOSDistance;
//}
//
//bool eTurret::hasLOS() const
//{
//	return m_hasLOS;
//}
//
//void eTurret::setLOSDistance(float distance)
//{
//	m_LOSDistance = distance;
//}
//
//void eTurret::setHasLOS(bool state)
//{
//	m_hasLOS = state;
//	m_LOSColor = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
//}

//void eTurret::setOrientation(const glm::vec2 orientation)
//{
//	m_orientation = orientation;
//}

void eTurret::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float eTurret::getRotation() const
{
	return m_rotationAngle;
}

void eTurret::m_Move()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;
	
	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if(abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}
	
	//getTransform()->position = m_pEnemy->getTransform()->position;

	/*getRigidBody()->acceleration = getOrientation() * getAccelerationRate();*/

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	/*getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);*/

	//getTransform()->position += getRigidBody()->velocity;
}
