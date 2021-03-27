#include "Target.h"


#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	
	const float deltaTime = 1.0f / 60.f;

	// Normalize direction vector
	float dirmagnitude = Util::magnitude(m_direction);
	if (dirmagnitude > 0) {
		getRigidBody()->acceleration = Util::normalize(m_direction) * ACCELERATION;

	}
	else if (Util::magnitude(getRigidBody()->velocity) > 0) {
		getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -ACCELERATION;
	}

	getRigidBody()->velocity += getRigidBody()->acceleration;

	// to stop the jittering, resets acceleration and velocity once velocity is low enough
	if (Util::magnitude(getRigidBody()->velocity) < ACCELERATION) {
		getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	}

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	getTransform()->position = pos;
}

void Target::clean()
{
}

void Target::moveLeft() {
	m_direction.x = -1;
}

void Target::moveRight() {
	m_direction.x = 1;
}

void Target::moveUp() {
	m_direction.y = -1;
}

void Target::moveDown() {
	m_direction.y = 1;
}

//void Target::stopMovingY() {
//	m_direction.y = 0;
//}
//
//void Target::stopMovingX() {
//	m_direction.x = 0;
//}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
