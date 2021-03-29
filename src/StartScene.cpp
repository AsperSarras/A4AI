#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	const SDL_Color black = { 0, 0, 0, 255 };
	m_pStartName = new Label("Asper Sarras.", "Consolas", 70, black, glm::vec2(360.0f, 70.0f));
	m_pStartName->setParent(this);
	addChild(m_pStartName);

	m_pStartStudentN = new Label("101324242", "Consolas", 60, black, glm::vec2(200.0f, 150.0f));
	m_pStartStudentN->setParent(this);
	addChild(m_pStartStudentN);

	m_pStartName2 = new Label("Nguyen Viet Dung.", "Consolas", 70, black, glm::vec2(360.0f, 220.0f));
	m_pStartName2->setParent(this);
	addChild(m_pStartName2);

	m_pStartStudentN2 = new Label("101304116", "Consolas", 60, black, glm::vec2(200.0f, 290.0f));
	m_pStartStudentN2->setParent(this);
	addChild(m_pStartStudentN2);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(400.0f, 400.0f);

	m_pStartButton->addEventListener(CLICK, [&]()-> void
		{
			m_pStartButton->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE);
		});

	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pStartButton->setAlpha(128);
		});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pStartButton->setAlpha(255);
		});
	addChild(m_pStartButton);


}