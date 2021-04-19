#include "Bullet.h"

#include "Game.h"
#include "Util.h"
#include "EventManager.h"

Bullet::Bullet(float rotation, glm::vec2 position, std::string texture, std::string key, bool enable)
{
	TextureManager::Instance()->load(texture, key);

	m_key = key;
	auto size = TextureManager::Instance()->getTextureSize(m_key);
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = position;
	setRotation(rotation);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	setType(BULLET);
	getRigidBody()->isColliding = false;
	setEnabled(enable);
	//setOrientation(orientation);
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	TextureManager::Instance()->draw(m_key,
		getTransform()->position.x, getTransform()->position.y,
		m_rotationAngle, 255, true);
}

void Bullet::update()
{
	//getTransform()->rotation = m_PlayerTurret->getTransform()->rotation;
	move();
	//reset();
}

void Bullet::handeEvents()
{

}

void Bullet::clean()
{

}

float Bullet::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Bullet::setAccelerationRate(float rate)
{
	m_accelerationRate = rate;
}

glm::vec2 Bullet::getOrientation() const
{
	return m_orientation;
}

void Bullet::setOrientation(glm::vec2 orientation)
{
	m_orientation = orientation;
}

float Bullet::getRotation() const
{
	return m_rotationAngle;
}

void Bullet::setRotation(float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

void Bullet::move()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();
	EventManager::Instance().update();

	//if (bulletShot == true) {

	setAccelerationRate(10.0f);
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
	//}
}

void Bullet::reset() {
	//if (getTransform()->position.x <= 0 && bulletShot == true) {
	//	bulletShot == false;
	//	getTransform()->position = glm::vec2(1000.0f, 1000.0f);
	//	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	//	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	//} 
	//if (getTransform()->position.x  >= 800 && bulletShot == true) {
	//	bulletShot == false;
	//	getTransform()->position = glm::vec2(1000.0f, 1000.0f);
	//	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	//	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	//}
	//if (getTransform()->position.y <= 0 && bulletShot == true) {
	//	bulletShot == false;
	//	getTransform()->position = glm::vec2(1000.0f, 1000.0f);
	//	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	//	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	//}
	//if (getTransform()->position.y >= 600 && bulletShot == true) {
	//	bulletShot == false;
	//	getTransform()->position = glm::vec2(1000.0f, 1000.0f);
	//	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	//	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	//}
}

void Bullet::Shoot()
{
	bulletShot = true;
}