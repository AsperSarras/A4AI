#pragma once
#ifndef __ETURRET__
#define __ETURRET__
#include "NavigationObject.h"
#include "TextureManager.h"
#include <glm/vec4.hpp>

class eTurret final : public NavigationObject
{
public:
	// constructor(s)
	eTurret();

	// destructor
	~eTurret();
	
	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	//glm::vec2 getOrientation() const override;
	//void setOrientation(glm::vec2 orientation) override;
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);

	//float getLOSDistance() const;
	//bool hasLOS()const;
	//
	//void setLOSDistance(float distance);
	//void setHasLOS(bool state);
	
private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	//glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	//float m_LOSDistance;
	//bool m_hasLOS;
	//glm::vec4 m_LOSColor;
	// private function
	void m_Move();
};

#endif /* defined (__SPACE_SHIP__) */