#pragma once
#ifndef __ETANK__
#define __ETANK__
#include "NavigationAgent.h"
#include "Line.h"

class ETank final : public NavigationAgent
{
public:
	// constructors
	ETank();
	
	// destructor
	~ETank();
	
	// life cycle functions
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
	void setStopRadius(float stopR);

	void turnLeft();
	//CheckPoints
	bool p0 = false;
	bool p1 = false;
	bool p2 = false;
	bool p3 = false;
	bool p4 = false;
	bool seek = false;
	bool avoidance = false;
	float cd = 0;
	float avocd = 0;
	
	float m_stopRadius;
	bool move = false;
	int moveCounterShip = 0;
	int ColObsL = 100;
	int ColObsR = 100;

	Line m_RWhishker;
	Line m_LWhishker;

	bool tLeft = false;
	bool tRight = false;

private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	//Line m_RWhishker;
	//Line m_LWhishker;
	
	// private function
	void m_Move();
private:

	const float speed = 2.0f;
};

#endif /* defined (__OBSTACLE__) */
