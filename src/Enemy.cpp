#include "Enemy.h"


#include "SoundManager.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Game.h"
#include "Util.h"

Enemy::Enemy(std::string texture, std::string key)
{
	TextureManager::Instance()->load(texture,key);
	//"../Assets/textures/Slime.png", "sl"
	m_key = key;
	auto size = TextureManager::Instance()->getTextureSize(m_key);
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	getRigidBody()->isColliding = false;

	setType(ETANK);
	setMaxSpeed(1.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(0.0f);
	setTurnRate(2.0f);
	setDetectionRadius(0.0f);

	setLOSDistance(250.0f);// 5 pixel per frame * 80 feet
	setLOSColor(glm::vec4(1, 0, 0, 1));//red

	setCurrentHp(4);
}

Enemy::~Enemy()
= default;

void Enemy::draw()
{
	TextureManager::Instance()->draw(m_key, 
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	//Util::DrawLine(m_RWhishker.Start(), m_RWhishker.End());
	//Util::DrawLine(m_LWhishker.Start(), m_LWhishker.End());

	////draw LOS
	/*Util::DrawLine(getTransform()->position, getTransform()->position + getOrientation() * getLOSDistance(), getLOSColour());*/
}

void Enemy::update()
{
	m_RWhishker.setLine(getTransform()->position,
		(getTransform()->position + Util::getOrientation(m_rotationAngle + 45) * 40.0f));
	m_LWhishker.setLine(getTransform()->position,
		(getTransform()->position + Util::getOrientation(m_rotationAngle + -45) * 40.0f));

		m_Move();

}

void Enemy::clean()
{
}

void Enemy::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

glm::vec2 Enemy::getDestination()
{
	return m_destination;
}

void Enemy::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

float Enemy::getTurnRate() const
{
	return m_turnRate;
}

void Enemy::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float Enemy::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Enemy::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void Enemy::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float Enemy::getRotation() const
{
	return m_rotationAngle;
}

void Enemy::setDetectionRadius(float stopR)
{
	m_detectionRadius = stopR;
}

float Enemy::getDetectionRadius() const
{
	return m_detectionRadius;
}

void Enemy::turnLeft()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

		if (patrol == true)
		{
			if (Util::distance(this->getTransform()->position, m_destination) < 200.0f)
			{
				setMaxSpeed(0.5f);
			}
			else if (Util::distance(this->getTransform()->position, m_destination) < 75.0f)
			{
				setMaxSpeed(0.0f);
			}
			else
			{
				setMaxSpeed(1.0f);
			}
		}
		//setTurnRate(5.0f);
		//setMaxSpeed(1.0f);
		// direction with magnitude
		m_targetDirection = m_destination - getTransform()->position;

		// normalized direction
		m_targetDirection = Util::normalize(m_targetDirection);

		auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

		auto turn_sensitivity = 5.0f;

		setRotation(getRotation() - getTurnRate());

		//getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

		//// using the formula pf = pi + vi*t + 0.5ai*t^2
		//getRigidBody()->velocity += getOrientation() * (deltaTime)+
		//	0.5f * getRigidBody()->acceleration * (deltaTime);

		getRigidBody()->velocity = m_targetDirection;

		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

		getTransform()->position += getRigidBody()->velocity;
}

void Enemy::setCurrentHp(int n)
{
	currentHp = n;
}

int Enemy::getCurrentHp() const
{
	return currentHp;
}

void Enemy::m_Move()
{
		auto deltaTime = TheGame::Instance()->getDeltaTime();

			//if (seek == true)
			//{
			//	if (Util::distance(this->getTransform()->position, m_destination) < 200.0f)
			//	{
			//		setMaxSpeed(0.75f);
			//	}
			//	else if (Util::distance(this->getTransform()->position, m_destination) < 75.0f)
			//	{
			//		setMaxSpeed(0.0f);
			//	}
			//	else if ((tLeft == false) && (tRight == false))
			//	{
			//		setMaxSpeed(1.0f);
			//	}
			//}

			if(flee==true)
			{
				m_targetDirection =  getTransform()->position - m_destination;
			}
			else
			{
				// direction with magnitude
				m_targetDirection = m_destination - getTransform()->position;
			}
			
			// normalized direction
			m_targetDirection = Util::normalize(m_targetDirection);

			auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

			auto turn_sensitivity = 5.0f;

			if ((tLeft == false) && (tRight == false))
			{
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
			}
			//TODO Heavily Polish This
			//Turn Left
			else if ((tLeft == true)/*&&(tRight==false)*/)
			{
				setMaxSpeed(1.5f);
				setTurnRate(10.0f);
				setRotation(getRotation() - getTurnRate());
				//if (getRotation()<-90.0f)
				//{
				//	setRotation(-90.0f);
				//}
			}
			//TODO Heavily Polish This
			//Turn Right
			else if (/*(tLeft == false) && */(tRight == true))
			{
				setMaxSpeed(1.5f);
				setTurnRate(10.0f);
				setRotation(getRotation() + getTurnRate());
				//if (getRotation() > 90.0f)
				//{
				//	setRotation(90.0f);
				//}
			}

			if (move == true)
			{
				//TODO Heavily Polish This
				if (avoidance == true)
				{
					setAccelerationRate(5.0f);
					getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
					// using the formula pf = pi + vi*t + 0.5ai*t^2
					getRigidBody()->velocity += getOrientation() * (deltaTime)+
						0.05f * getRigidBody()->acceleration * (deltaTime);
				}
				else
					getRigidBody()->velocity = m_targetDirection;

					getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

					getTransform()->position += getRigidBody()->velocity;
			}
}
