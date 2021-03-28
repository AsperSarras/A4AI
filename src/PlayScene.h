#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Bullet.h"
#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "ETank.h"
#include "eTurret.h"
#include "Target.h"
#include "PlayerTank.h"
#include "pTurret.h"
#include <vector>


#include "DecisionTree.h"
#include "Tile.h"
#include "TiledLevel.h"


class pTurret;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	float GameTimer = 0;
	float GunCD = 0;
	float ButtonCD = 0;
	int EnemiesDestroyed = 0;
	int TotalBullets = 0;
	int TotalEBullets = 0;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;

	glm::vec2 m_Position;
	SDL_Point m_mousePosition;


	//Tiles
	void m_setGridEnabled(bool state) const;
	std::vector<Tile*> m_pGrid;
	std::vector<NavigationAgent*>m_pMap;
	void m_buildGrid();
	//Map
	TileC* Bg;
	TileC* m_field[6];
	//convenience functions
	Tile* m_getTile(int col, int row) const;
	Tile* m_getTile(glm::vec2 grid_position) const;


	//Labels
	Label* m_Inst[5];

	//Enemy
	ETank* m_pEnemyTank[8];
	eTurret* m_pETurret[8];
	void m_move();
	//Player
	PlayerTank* m_pPlayerTank;
	pTurret* m_pPlayerTurret;
	//Bullets
	std::vector<Bullet*>m_pBullet;
	std::vector<Bullet*>m_pEnemyBullet;

	void m_CheckShipLOS(NavigationAgent* object);

	//Decision tree
	DecisionTree* decisionTree;
	
	// Others
	bool Debug = false;

};

#endif /* defined (__PLAY_SCENE__) */