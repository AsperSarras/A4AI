#pragma once
#ifndef __WIN_SCENE__
#define __WIN_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"

class WinScene final : public Scene
{
public:
	WinScene();
	~WinScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	Label* m_pStartLabel{};
	Label* m_pInstructionsLabel{};

	Button* m_pStartButton;
	Button* m_pExitButton;
};

#endif /* defined (__START_SCENE__) */
