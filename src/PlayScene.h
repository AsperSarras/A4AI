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



#include "CloseCombatEnemy.h"
#include "Condition.h"
#include "DecisionTree.h"
#include "Hp.h"
#include "PathNode.h"
#include "RangedCombatEnemy.h"
#include "StateMachine.h"
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
	std::vector<DisplayObject*>m_pMap;
	void m_buildGrid();
	//
	std::vector<PathNode*> m_pSGrid;
	void m_buildGridSight();
	void m_CheckPathNodeLOS();
	//bool m_gridVisible;
	void m_toggleGrid(bool state);
	//Map
	TileC* Bg;
	int obstacles = 6;
	TileC* m_field[6];
	int dest = 2;
	DestructibleObstacle* m_dField[2];
	//convenience functions
	Tile* m_getTile(int col, int row) const;
	Tile* m_getTile(glm::vec2 grid_position) const;


	//Labels
	Label* m_Inst[7];

	//Enemy
	int Enemies = 2;
	Enemy* m_pEnemy[2];
	CloseCombatEnemy* m_pCloseCombatEnemy;
	RangedCombatEnemy* m_pRangedEnemy;
	EnemyDebugMode* m_pEnemyDebug[2];
	void m_move();
	//Player
	PlayerAgent* m_pPlayer;
	//EnemyDebugMode* m_pEnemyDebug0;
	//Bullets
	std::vector<Bullet*>m_pBullet;
	//std::vector<Bullet*>m_pEnemyBullet;
	
	void m_CheckShipLOS(NavigationAgent* from, DisplayObject* to);
	//void m_CheckShipLOS(NavigationAgent* object);
	void m_CheckShipCloseCombatPlayer(NavigationAgent* object);

	//StateMachine
		// State machine properties
	//CloseCombat
	Condition* m_pCloseCombatHasLOSCondition;
	Condition* m_pCloseCombatLostLOSCondition;
	Condition* m_pCloseCombatIsWithinDetectionRadiusCondition;
	Condition* m_pCloseCombatIsNotWithinDetectionRadiusCondition;
	Condition* m_pCloseCombatIsWithinCombatRangeCondition;
	Condition* m_pCLoseCombatLifeIsLow;
	//Condition* m_pIsNotWithinCombatRangeCondition;
	//Ranged


	StateMachine* m_pCloseCombatStateMachine;
	void m_buildCloseCombatStateMachine();

	StateMachine* m_pRangedStateMachine;
	void m_buildRangedStateMachine();
	
	////Decision tree
	//DecisionTree* decisionTree[6];
	
	// Others
	bool Debug = false;
	//Hp
	Hp* PlayerHp[3];
	Hp* Enemy0[2];
	Hp* Enemy1[2];
	Hp* Tree1[3];
	Hp* Tree2[3];

};

#endif /* defined (__PLAY_SCENE__) */