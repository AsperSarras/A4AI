#include "PlayerTank.h"

#include "Game.h"
#include "Util.h"
#include "EventManager.h"

PlayerTank::PlayerTank()
{
	TextureManager::Instance()->load("../Assets/textures/tiger1.png", "Tiger");

	auto size = TextureManager::Instance()->getTextureSize("Tiger");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER_TANK);
	setMaxWSpeed(60.0f);
	setMaxSSpeed(60.0f);
	setMaxASpeed(60.0f);
	setMaxDSpeed(60.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(00.0f);
	setTurnRate(2.0f);
	setCurrentHp(00.0f);

	setCloseCombatDistance(60.0f);// 5 pixel per frame * 80 feet
	setLOSColor(glm::vec4(1, 0, 0, 1));//red
}

PlayerTank::~PlayerTank()
= default;

void PlayerTank::draw()
{
	TextureManager::Instance()->draw("Tiger",
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	Util::DrawLine(getTransform()->position, getTransform()->position + getOrientation() * getCloseCombatDistance(), getCloseCombatColour());
}

void PlayerTank::update()
{
	m_Move();
	m_checkBounds();
}

void PlayerTank::handleEvents()
{
	
}


void PlayerTank::clean()
{
}

void PlayerTank::setDestination(glm::vec2 destination)
{
	m_destination = destination;
}

void PlayerTank::setMaxWSpeed(float speed)
{
	m_maxWSpeed = speed;
}

void PlayerTank::setMaxSSpeed(float speed)
{
	m_maxSSpeed = speed;
}

void PlayerTank::setMaxASpeed(float speed)
{
	m_maxASpeed = speed;
}

void PlayerTank::setMaxDSpeed(float speed)
{
	m_maxDSpeed = speed;
}

float PlayerTank::getRotation() const
{
	return m_rotationAngle;
}

void PlayerTank::setRotation(float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float PlayerTank::getTurnRate() const
{
	return m_turnRate;
}

void PlayerTank::setTurnRate(float rate)
{
	m_turnRate = rate;
}

float PlayerTank::getAccelerationRate() const
{
	return m_accelerationRate;
}

void PlayerTank::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void PlayerTank::wCollision()
{
	setMaxWSpeed(0);
}


float PlayerTank::getCurrentHp() const
{
	return currentHp;
}

void PlayerTank::setCurrentHp(float hpValue)
{
	currentHp = hpValue;
}

void PlayerTank::m_Move()
{
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
	
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			getTransform()->position.y -= (m_maxWSpeed * deltaTime);
			////setAccelerationRate(2.0f);
			//getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

			//// using the formula pf = pi + vi*t + 0.5ai*t^2
			//getRigidBody()->velocity += getOrientation()/* * (deltaTime)+
			//0.5f * getRigidBody()->acceleration * (deltaTime)*/;

			//getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxWSpeed);

			//Util::clamp(getRigidBody()->velocity, m_maxWSpeed);

			//getTransform()->position.x += getRigidBody()->velocity;

		}
		else
		{
			/*getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);*/
		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			getTransform()->position.y += (m_maxSSpeed * deltaTime);
			//setAccelerationRate(-25.0f);
			//setMaxWSpeed(1.0f);
			//getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

			//// using the formula pf = pi + vi*t + 0.5ai*t^2
			//getRigidBody()->velocity += getOrientation()/* * (deltaTime)+
			//0.5f * getRigidBody()->acceleration * (deltaTime)*/;

			//getRigidBody()->velocity = -Util::clamp(getRigidBody()->velocity, m_maxSSpeed);

			//getTransform()->position += getRigidBody()->velocity;

		}

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			getTransform()->position.x += (m_maxDSpeed * deltaTime);
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			getTransform()->position.x -= (m_maxASpeed * deltaTime);
		}
}

void PlayerTank::m_checkBounds()
	{
		if (getTransform()->position.x > Config::SCREEN_WIDTH)
		{
			getTransform()->position = glm::vec2(799.0f, getTransform()->position.y);
		}
	
		if (getTransform()->position.x < 0)
		{
			getTransform()->position = glm::vec2(1.0f, getTransform()->position.y);
		}
	
		if (getTransform()->position.y > Config::SCREEN_HEIGHT)
		{
			getTransform()->position = glm::vec2(getTransform()->position.x, 599.0f);
		}
	
		if (getTransform()->position.y < 0)
		{
			getTransform()->position = glm::vec2(getTransform()->position.x, 1.0f);
		}
	}