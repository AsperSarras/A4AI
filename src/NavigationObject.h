#pragma once
#ifndef _NAVIGATION_OBJECT_
#define _NAVIGATION_OBJECT_
#include <GLM/detail/type_vec4.hpp>

#include "DisplayObject.h"

class NavigationObject :public DisplayObject
{
public:
	NavigationObject();

	~NavigationObject();

	virtual void draw() override = 0;
	virtual void update() override = 0;
	virtual void clean() override = 0;

	glm::vec2 getGridPosition() const;
	void setGridPosition(float col, float row);

	glm::vec2 getOrientation() const { return m_orientation; }
	void setOrientation(glm::vec2 orientation) { m_orientation = orientation; }

	float getLOSDistance() const;
	bool hasLOS()const;

	void setLOSDistance(float distance);
	void setHasLOS(bool state);

	glm::vec4 m_LOSColor;
private:
	glm::vec2 m_gridPosition;
	glm::vec2 m_orientation;
	
	float m_LOSDistance;
	bool m_hasLOS;
	//glm::vec4 m_LOSColor;
};

#endif