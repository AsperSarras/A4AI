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

	//Grid LOS
	std::vector<PathNode*> m_pSGrid;
	std::vector<PathNode*> m_pLeftTreeNodes;
	std::vector<PathNode*> m_pRightTreeNodes;
	void m_buildGridSight();
	void m_CheckPathNodeLOS();
	void m_toggleGrid(bool state);
	PathNode* m_findClosestPathNodeWithLOS(NavigationAgent* agent);
	PathNode* m_findClosestPathNodeWithoutLOS(NavigationAgent* agent);
	
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

	//Bullets
	std::vector<Bullet*>m_pBullet;
	std::vector<Bullet*>m_pEnemyBullet;
	
	void m_CheckShipLOS(NavigationAgent* from, DisplayObject* to);
	void m_CheckCloseCombat(NavigationAgent* from,DisplayObject* to);
	void m_CheckRangedCombat(NavigationAgent* from, DisplayObject* to);
	

	//StateMachine
	//CloseCombat
	Condition* m_pCloseCombatHasLOSCondition;
	Condition* m_pCloseCombatLostLOSCondition;
	Condition* m_pCloseCombatIsWithinDetectionRadiusCondition;
	Condition* m_pCloseCombatIsNotWithinDetectionRadiusCondition;
	Condition* m_pCloseCombatIsWithinCombatRangeCondition;
	Condition* m_pCLoseCombatLifeIsLow;
	Condition* m_pCloseCombatNotWithinCombatRangeCondition;
	//Ranged
	Condition* m_pRangedHasLOSCondition;
	Condition* m_pRangedLostLOSCondition;
	Condition* m_pRangedIsWithinDetectionRadiusCondition;
	Condition* m_pRangedIsNotWithinDetectionRadiusCondition;
	Condition* m_pRangedIsWithinCombatRangeCondition;
	Condition* m_pRangedLifeIsLow;
	Condition* m_pRangedNotWithinCombatRangeCondition;
	Condition* m_pRangedIsHit;
	Condition* m_pRangedCoverWait;
	Condition* m_pRangedCoverOut;


	void m_buildCloseCombatStateMachine();
	StateMachine* m_pCloseCombatStateMachine;
	StateMachine* m_pCloseCombatStateMachine2;
	
	void m_buildRangedStateMachine();
	StateMachine* m_pRangedStateMachine;
	
	////Decision tree
	//DecisionTree* decisionTree[6];
	
	// Others
	bool Debug = false;
	//Hp
	Hp* PlayerHp[5];
	Hp* Enemy0[4];
	Hp* Enemy1[4];
	Hp* Tree1[3];
	Hp* Tree2[3];

};

#endif /* defined (__PLAY_SCENE__) */