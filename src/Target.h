#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingY();
	void stopMovingX();

private:
	void m_checkBounds();
	void m_reset();
	const float ACCELERATION = 10.0f;
	glm::vec2 m_direction;
};


#endif /* defined (__TARGET__) */