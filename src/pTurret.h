#pragma once

#ifndef __PTURRET__
#define __PTURRET__
#include "NavigationAgent.h"
#include "TextureManager.h"
#include "ETank.h"

class pTurret : public NavigationAgent
{
public:
	// constructor(s)
	pTurret(ETank* base/*glm::vec2 object,float radius*/);

	// destructor
	~pTurret();

	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
	
	void setDetectionRadius(float stopR);
	float getDetectionRadius()const;

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

private:

	float m_detectionRadius;

	// private function
	void m_Move();
};

#endif /* defined (__SPACE_SHIP__) */
