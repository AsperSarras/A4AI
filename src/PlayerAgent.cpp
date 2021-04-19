#include "PlayerAgent.h"

#include "Game.h"
#include "Util.h"
#include "EventManager.h"

PlayerAgent::PlayerAgent()
{
	TextureManager::Instance()->load("../Assets/textures/Best.png", "bst");

	auto size = TextureManager::Instance()->getTextureSize("bst");

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
	setTurnRate(4.0f);
	setCurrentHp(00.0f);

	setCloseCombatDistance(121.0f);// 5 pixel per frame * 80 feet
	setCloseCombatColour(glm::vec4(1, 0, 0, 1));//red
}

PlayerAgent::~PlayerAgent()
= default;

void PlayerAgent::draw()
{
	TextureManager::Instance()->draw("bst",
		getTransform()->position.x, getTransform()->position.y, getRotation(), 255, true);

	//Util::DrawLine(getTransform()->position, getTransform()->position + getOrientation() * getCloseCombatDistance(), getCloseCombatColour());
}

void PlayerAgent::update()
{
	m_Move();
	m_checkBounds();
}

void PlayerAgent::handleEvents()
{
	
}


void PlayerAgent::clean()
{
}

void PlayerAgent::setDestination(glm::vec2 destination)
{
	m_destination = destination;
}

void PlayerAgent::setMaxWSpeed(float speed)
{
	m_maxWSpeed = speed;
}

void PlayerAgent::setMaxSSpeed(float speed)
{
	m_maxSSpeed = speed;
}

void PlayerAgent::setMaxASpeed(float speed)
{
	m_maxASpeed = speed;
}

void PlayerAgent::setMaxDSpeed(float speed)
{
	m_maxDSpeed = speed;
}

//float PlayerAgent::getRotation() const
//{
//	return m_rotationAngle;
//}
//
//void PlayerAgent::setRotation(float angle)
//{
//	m_rotationAngle = angle;
//
//	const auto offset = -90.0f;
//	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;
//
//	const auto x = cos(angle_in_radians);
//	const auto y = sin(angle_in_radians);
//
//	// convert the angle to a normalized vector and store it in Orientation
//	setOrientation(glm::vec2(x, y));
//}

float PlayerAgent::getTurnRate() const
{
	return m_turnRate;
}

void PlayerAgent::setTurnRate(float rate)
{
	m_turnRate = rate;
}

float PlayerAgent::getAccelerationRate() const
{
	return m_accelerationRate;
}

void PlayerAgent::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

void PlayerAgent::wCollision()
{
	setMaxWSpeed(0);
}


int PlayerAgent::getCurrentHp() const
{
	return currentHp;
}

void PlayerAgent::setCurrentHp(int hpValue)
{
	currentHp = hpValue;
}

void PlayerAgent::m_Move()
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

void PlayerAgent::m_checkBounds()
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