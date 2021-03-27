#pragma once
#ifndef __AGENTS__
#define __AGENTS__

#include <glm/vec4.hpp>
#include "NavigationAgent.h"

class Agents : public NavigationAgent
{
public:
	Agents();
	~Agents();

	// Inherited via GameObject
	void draw() override =0;
	void update() override=0;
	void clean() override=0;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	float getLOSDistance() const;
	bool hasLOS()const;
	float getCurrentHeading()const;
	glm::vec4 getLOSColour()const;

	// setters
	void setTargetPosition(glm::vec2 new_Position);
	void setCurrentDirection(glm::vec2 new_Direction);
	void setLOSDistance(float distance);
	void setHasLOS(bool state);
	void setCurrentHeading(float heading);
	void setLOSColor(glm::vec4 color);

private: 
	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	//Line of sight
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColor;
};
#endif
