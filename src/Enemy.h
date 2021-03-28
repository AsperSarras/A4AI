#pragma once
#ifndef __ENEMY__
#define __ENEMY__
#include "DecisionTree.h"
#include "NavigationAgent.h"
#include "Line.h"

class Enemy final : public NavigationAgent
{
public:
	// constructors
	Enemy();
	
	// destructor
	~Enemy();
	
	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(glm::vec2 destination);
	glm::vec2 getDestination();
	void setMaxSpeed(float speed);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
	void setDetectionRadius(float stopR);
	float getDetectionRadius()const;

	void turnLeft();

	void setCurrentHp(int n);
	int getCurrentHp() const;
	
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
	

	bool move = true;
	int moveCounterShip = 0;
	int ColObsL = 100;
	int ColObsR = 100;

	Line m_RWhishker;
	Line m_LWhishker;

	bool tLeft = false;
	bool tRight = false;
	//DecisionTree* decisionTree;
private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;
	float m_rotationAngle;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	float m_detectionRadius;
	
	int currentHp;
	
	//Line m_RWhishker;
	//Line m_LWhishker;

	//DecisionTree* decisionTree;
	
	// private function
	void m_Move();
private:

	const float speed = 2.0f;
};

#endif /* defined (__OBSTACLE__) */
