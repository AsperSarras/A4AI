#include "Hp.h"
#include "TextureManager.h"

Hp::Hp()
{
	TextureManager::Instance()->load("../Assets/textures/Hp.png", "hp");

	auto size = TextureManager::Instance()->getTextureSize("hp");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	setEnabled(true);
}

Hp::~Hp()
= default;


void Hp::draw()
{
	TextureManager::Instance()->draw("hp",
		getTransform()->position.x, getTransform()->position.y,
		0, 255, true);
}

void Hp::update()
{

}

void Hp::destroy()
{
	setEnabled(false);
}
