#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Bullet.h"
#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Enemy.h"
#include "eTurret.h"
#include "Target.h"
#include "PlayerAgent.h"
#include "EnemyDebugMode.h"
#include <vector>


#include "DecisionTree.h"
#include "Hp.h"
#include "Tile.h"
#include "TiledLevel.h"


class EnemyDebugMode;

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
	float CheckCD = 0;
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
	std::vector<Tile*> m_MovementNodes;
	std::vector<NavigationAgent*>m_pMap;
	void m_buildGrid();
	//Map
	TileC* Bg;
	int obstacles = 6;
	TileC* m_field[6];
	//convenience functions
	Tile* m_getTile(int col, int row) const;
	Tile* m_getTile(glm::vec2 grid_position) const;


	//Labels
	Label* m_Inst[7];

	//Enemy
	int Enemies = 6;
	Enemy* m_pEnemy[6];
	EnemyDebugMode* m_pEnemyDebug[6];
	void m_move();
	//Player
	PlayerAgent* m_pPlayer;
	//EnemyDebugMode* m_pEnemyDebug0;
	//Bullets
	std::vector<Bullet*>m_pBullet;
	//std::vector<Bullet*>m_pEnemyBullet;

	void m_CheckShipLOS(NavigationAgent* object);
	void m_CheckShipCloseCombatPlayer(NavigationAgent* object);

	//Decision tree
	DecisionTree* decisionTree[6];
	
	// Others
	bool Debug = false;
	//Hp
	Hp* PlayerHp[3];
	Hp* Enemy0;
	Hp* Enemy1;
	Hp* Enemy2[2];
	Hp* Enemy3[2];
	Hp* Enemy4[2];
	Hp* Enemy5[2];

};

#endif /* defined (__PLAY_SCENE__) */