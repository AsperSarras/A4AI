#include "pTurret.h"

#include "EventManager.h"
#include "Game.h"
#include "Util.h"

pTurret::pTurret(ETank* base/*glm::vec2 object, float radius*/)
{
	//TextureManager::Instance()->load("../Assets/textures/TigerT.png", "TigerT");

	//auto size = TextureManager::Instance()->getTextureSize("TigerT");
	setWidth(base->getWidth());
	setHeight(base->getHeight());

	getTransform()->position = base->getTransform()->position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PTURRET);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(0.0f);
	setTurnRate(2.0f);
	
	setDetectionRadius(base->getDetectionRadius());
	setLOSDistance(base->getLOSDistance());
	setLOSColor(glm::vec4(1, 0, 0, 1));//red

	
}

pTurret::~pTurret()
= default;

void pTurret::draw()
{
	//TextureManager::Instance()->draw("TigerT",
	//	getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);
	Util::DrawLine(getTransform()->position, getTransform()->position + getOrientation() * getLOSDistance(), getLOSColour());
	Util::DrawCircle(getTransform()->position, getDetectionRadius(),glm::vec4(1,0,0,1));
	//Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f));
}

void pTurret::update()
{
	m_Move();
}

void pTurret::clean()
{
}

void pTurret::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void pTurret::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

float pTurret::getTurnRate() const
{
	return m_turnRate;
}

void pTurret::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float pTurret::getAccelerationRate() const
{
	return m_accelerationRate;
}

void pTurret::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void pTurret::setDetectionRadius(float stopR)
{
	m_detectionRadius = stopR;
}

float pTurret::getDetectionRadius() const
{
	return m_detectionRadius;
}

void pTurret::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float pTurret::getRotation() const
{
	return m_rotationAngle;
}

void pTurret::m_Move()
{
	EventManager::Instance().update();
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;

	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
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



	/*if (EventManager::Instance().isKeyDown(SDL_SCANCODE_RIGHT))
	{
		setTurnRate(1.5f);
		setRotation(getRotation() + getTurnRate());
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_LEFT))
	{
		setTurnRate(-1.5f);
		setRotation(getRotation() + getTurnRate());
	}*/










	//getTransform()->position = m_pEnemyTank->getTransform()->position;

	/*getRigidBody()->acceleration = getOrientation() * getAccelerationRate();*/

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	/*getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);*/

	//getTransform()->position += getRigidBody()->velocity;
}