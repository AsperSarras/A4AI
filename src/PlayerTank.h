#pragma
#ifndef __PLAYER_TANK__
#define __PLAYER_TANK__

#include "DisplayObject.h"
#include "TextureManager.h"
#include "NavigationObject.h"

class PlayerTank: public NavigationObject
{
public:
	PlayerTank();

	~PlayerTank();
	
	void draw() override;
	void update() override;
    void handleEvents();
	void clean() override;

	void setMaxWSpeed(float speed);
	void setMaxSSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);

	void wCollision();

	
	glm::vec2 m_orientation;
	float m_rotationAngle;
	//
	//glm::vec2 m_Wposition;
	//glm::vec2 m_Sposition;
private:
	//glm::vec2 m_orientation;
	//float m_rotationAngle;
	float m_maxWSpeed;
	float m_maxSSpeed;
	float m_turnRate;
	float m_accelerationRate;

	void m_Move();
	void m_checkBounds();
};

#endif
