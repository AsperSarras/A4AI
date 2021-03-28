#pragma once
#ifndef _HP_
#define _HP_
#include "DisplayObject.h"

class Hp :public DisplayObject
{
public:
	Hp();

	~Hp();

	void draw() override;
	void update() override;
	void clean() override {};

	void destroy();
private:

};
#endif