#pragma once
#ifndef __PLAYER_AGENT__
#define __PLAYER_AGENT__

#include "DisplayObject.h"
#include "TextureManager.h"
#include "NavigationAgent.h"

class PlayerAgent: public NavigationAgent
{
public:
	PlayerAgent();

	~PlayerAgent();
	
	void draw() override;
	void update() override;
    void handleEvents();
	void clean() override;

	void setDestination(glm::vec2 destination);
	void setMaxWSpeed(float speed);
	void setMaxSSpeed(float speed);
	void setMaxASpeed(float speed);
	void setMaxDSpeed(float speed);

	//float getRotation() const;
	//void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);

	void wCollision();

	int getCurrentHp() const;
	void setCurrentHp(int n);

	
	glm::vec2 m_orientation;
	//float m_rotationAngle;
	//
	//glm::vec2 m_Wposition;
	//glm::vec2 m_Sposition;
private:
	//glm::vec2 m_orientation;
	//float m_rotationAngle;
	float m_maxWSpeed;
	float m_maxSSpeed;
	float m_maxASpeed;
	float m_maxDSpeed;
	float m_turnRate;
	float m_accelerationRate;
	int currentHp;
	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	
	void m_Move();
	void m_checkBounds();
};

#endif
