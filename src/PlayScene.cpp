#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include"DecisionTree.h"


// required for IMGUI
#include "CloseCombatAttack.h"
#include "Flee.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "MoveBehindCover.h"
#include "MoveToLOS.h"
#include "MoveToPlayer.h"
#include "Patrol.h"
#include "RangedAttack.h"
#include "Renderer.h"
#include "Transition.h"
#include "Util.h"
#include "WaitBehindCover.h"

PlayScene::PlayScene()
{
	PlayScene::start();

	SoundManager::Instance().load("../Assets/audio/Bgm2.mp3", "Bgm2", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/CloseCombatAttack.mp3", "at", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Damage.mp3", "dmg", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/gunShot.mp3", "sht", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Exp.wav", "Expl", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Die.mp3", "die", SOUND_SFX);
	SoundManager::Instance().playMusic("Bgm2", -1, 0);
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{	
	auto deltaTime = TheGame::Instance()->getDeltaTime();
	updateDisplayList();

	//LOS,CloseCombatRange
	for (int i = 0; i < Enemies; i++)
	{
		if(m_pEnemy[i]->isEnabled())
			m_CheckShipLOS(m_pEnemy[i],m_pPlayer);
		//if (m_pEnemy[i]->isEnabled())
		//	m_CheckCloseCombat(m_pEnemy[i], m_pPlayer);
		//if (m_pEnemy[i]->isEnabled())
		//	m_CheckRangedCombat(m_pEnemy[i], m_pPlayer);
		if (m_pEnemyDebug[i]->isEnabled())
			m_CheckShipLOS(m_pEnemyDebug[i],m_pPlayer);
		if (m_pEnemyDebug[i]->isEnabled())
			m_CheckCloseCombat(m_pEnemyDebug[i], m_pPlayer);
		if (m_pEnemyDebug[i]->isEnabled())
			m_CheckRangedCombat(m_pEnemyDebug[i], m_pPlayer);
		
		m_CheckCloseCombat(m_pPlayer,m_pEnemy[i]);
	}
	//CloseCombatRange DestObj
	for (int i = 0; i < dest; i++)
	{
		m_CheckCloseCombat(m_pPlayer,m_dField[i]);
	}
	//TilesLos
	m_CheckPathNodeLOS();

	//State Machine Conditions and Updates
	for (int i = 0; i < Enemies; i++)
	{
		////StateMachineCloseCombat1
		if(i==0)
		{
			// Set conditions
			m_pCloseCombatHasLOSCondition->SetCondition(m_pEnemy[i]->hasLOS());
			m_pCloseCombatLostLOSCondition->SetCondition(!m_pEnemy[i]->hasLOS());
			m_pCloseCombatIsWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getDetectionRadius()
			);
			m_pCloseCombatIsNotWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getDetectionRadius()
			);
			m_pCloseCombatIsWithinCombatRangeCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getCloseCombatDistance()
			);
			m_pCLoseCombatLifeIsLow->SetCondition(m_pEnemy[i]->getCurrentHp() == 1);
			m_pCloseCombatNotWithinCombatRangeCondition->SetCondition(Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getCloseCombatDistance());
			m_pCloseCombatStateMachine->Update();
		}
		////StateMachineRanged1
		else if(i==1)
		{
			// Set conditions
			m_pRangedHasLOSCondition->SetCondition(m_pEnemy[i]->hasLOS());
			m_pRangedLostLOSCondition->SetCondition(!m_pEnemy[i]->hasLOS());
			m_pRangedIsWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getDetectionRadius()
			);
			m_pRangedIsNotWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getDetectionRadius()
			);
			m_pRangedIsWithinCombatRangeCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getRangedCombatDistance()
			);
			m_pRangedLifeIsLow->SetCondition(m_pEnemy[i]->getCurrentHp() == 1);

			m_pRangedNotWithinCombatRangeCondition->SetCondition(Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getRangedCombatDistance());
			m_pRangedIsHit->SetCondition(m_pEnemy[i]->isHit == true);
			m_pRangedCoverWait->SetCondition(m_pEnemy[i]->isCovering == true);
			m_pRangedCoverOut->SetCondition(m_pEnemy[i]->isCovering == false);
			m_pRangedStateMachine->Update();
		}
		////StateMachineCloseCombat1
		else if (i == 2)
		{
			// Set conditions
			m_p2CloseCombatHasLOSCondition->SetCondition(m_pEnemy[i]->hasLOS());
			m_p2CloseCombatLostLOSCondition->SetCondition(!m_pEnemy[i]->hasLOS());
			m_p2CloseCombatIsWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getDetectionRadius()
			);
			m_p2CloseCombatIsNotWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getDetectionRadius()
			);
			m_p2CloseCombatIsWithinCombatRangeCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getCloseCombatDistance()
			);
			m_p2CLoseCombatLifeIsLow->SetCondition(m_pEnemy[i]->getCurrentHp() == 1);
			m_p2CloseCombatNotWithinCombatRangeCondition->SetCondition(Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getCloseCombatDistance());
			m_p2CloseCombatStateMachine->Update();
		}
		////StateMachineRanged2
		else if (i == 3)
		{
			// Set conditions
			m_p2RangedHasLOSCondition->SetCondition(m_pEnemy[i]->hasLOS());
			m_p2RangedLostLOSCondition->SetCondition(!m_pEnemy[i]->hasLOS());
			m_p2RangedIsWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getDetectionRadius()
			);
			m_p2RangedIsNotWithinDetectionRadiusCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getDetectionRadius()
			);
			m_p2RangedIsWithinCombatRangeCondition->SetCondition(
				Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy[i]->getRangedCombatDistance()
			);
			m_p2RangedLifeIsLow->SetCondition(m_pEnemy[i]->getCurrentHp() == 1);

			m_p2RangedNotWithinCombatRangeCondition->SetCondition(Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) > m_pEnemy[i]->getRangedCombatDistance());
			m_p2RangedIsHit->SetCondition(m_pEnemy[i]->isHit == true);
			m_p2RangedCoverWait->SetCondition(m_pEnemy[i]->isCovering == true);
			m_p2RangedCoverOut->SetCondition(m_pEnemy[i]->isCovering == false);
			m_p2RangedStateMachine->Update();
		}
	}
	
	////Enemy trees
	//for (int i = 0; i < Enemies; i++)
	//{
	//	decisionTree[i]->MakeDecision();
	//}

	//Enemy movevents
	m_move();

	//Timer for Cooldowns
	{
		GameTimer += 1 * deltaTime;
		ButtonCD += 1 * deltaTime;
		CheckCD += 1 * deltaTime;
		GunCD += 1 * deltaTime;
		RespawnCD += 1 * deltaTime;
		
		m_pEnemy[0]->AttackCd += 1 * deltaTime;
		
		m_pEnemy[1]->AttackCd += 1 * deltaTime;
		m_pEnemy[1]->CoveringTime += 1 * deltaTime;

		m_pEnemy[2]->AttackCd += 1 * deltaTime;

		m_pEnemy[3]->AttackCd += 1 * deltaTime;
		m_pEnemy[3]->CoveringTime += 1 * deltaTime;
	}

	for (auto i = 0; i < Enemies; i++)
	{
		//m_pEnemy[i]->cd += 1 * deltaTime;
		m_pEnemy[i]->avocd += 1 * deltaTime;
	}
	//if ((m_pPlayer->isEnabled() == false)||(EnemiesDestroyed==8))
	//{
	//	ButtonCD += 1 * deltaTime;
	//}
	//std::cout << GameTimer << std::endl;
	
	//Enemies Debug bind
	for (int i = 0; i < Enemies; i++)
	{
		m_pEnemyDebug[i]->getTransform()->position = m_pEnemy[i]->getTransform()->position;
		m_pEnemyDebug[i]->setDestination(m_pEnemy[i]->getDestination());
	}

	//Hp Bind
	{
		// Enemy0
		Enemy0[0]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x,m_pEnemy[0]->getTransform()->position.y - 40 };
		Enemy0[1]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x + 10,m_pEnemy[0]->getTransform()->position.y - 40 };
		Enemy0[2]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x - 10,m_pEnemy[0]->getTransform()->position.y - 40 };
		Enemy0[3]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x - 20,m_pEnemy[0]->getTransform()->position.y - 40 };

		// Enemy1
		Enemy1[0]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x,m_pEnemy[1]->getTransform()->position.y - 40 };
		Enemy1[1]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x + 10,m_pEnemy[1]->getTransform()->position.y - 40 };
		Enemy1[2]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x - 10,m_pEnemy[1]->getTransform()->position.y - 40 };
		Enemy1[3]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x - 20,m_pEnemy[1]->getTransform()->position.y - 40 };

		// Enemy 2
		Enemy2[0]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x,m_pEnemy[2]->getTransform()->position.y - 40 };
		Enemy2[1]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x + 10,m_pEnemy[2]->getTransform()->position.y - 40 };
		Enemy2[2]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x - 10,m_pEnemy[2]->getTransform()->position.y - 40 };
		Enemy2[3]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x - 20,m_pEnemy[2]->getTransform()->position.y - 40 };

		// Enemy 3
		Enemy3[0]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x,m_pEnemy[3]->getTransform()->position.y - 40 };
		Enemy3[1]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x + 10,m_pEnemy[3]->getTransform()->position.y - 40 };
		Enemy3[2]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x - 10,m_pEnemy[3]->getTransform()->position.y - 40 };
		Enemy3[3]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x - 20,m_pEnemy[3]->getTransform()->position.y - 40 };
		
		//Player hp bind
		PlayerHp[0]->getTransform()->position = { m_pPlayer->getTransform()->position.x,m_pPlayer->getTransform()->position.y - 40 };
		PlayerHp[1]->getTransform()->position = { m_pPlayer->getTransform()->position.x + 10,m_pPlayer->getTransform()->position.y - 40 };
		PlayerHp[2]->getTransform()->position = { m_pPlayer->getTransform()->position.x - 10,m_pPlayer->getTransform()->position.y - 40 };
		PlayerHp[3]->getTransform()->position = { m_pPlayer->getTransform()->position.x - 20,m_pPlayer->getTransform()->position.y - 40 };
		PlayerHp[4]->getTransform()->position = { m_pPlayer->getTransform()->position.x + 20,m_pPlayer->getTransform()->position.y - 40 };
	}
	
	//Set Player destiantion
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	m_pPlayer->setDestination(glm::vec2(mx, my));

	//Player Bullet Off Screen
	for (int i = 0; i < m_pBullet.size(); i++)
	{
		if (m_pBullet[i]->getTransform()->position.x >= 800.0f ||
			m_pBullet[i]->getTransform()->position.x <= 0.0f ||
			m_pBullet[i]->getTransform()->position.y >= 600.0f ||
			m_pBullet[i]->getTransform()->position.y <= 0)
		{
			m_pBullet[i]->setEnabled(false);
			break;
		}
	}

	//Labels Switch
	//for (int i = 0; i < 7;i++)
	//{
	//	if (i==EnemiesDestroyed)
	//	{
	//		m_Inst[i]->setEnabled(true);
	//	}
	//	else
	//		m_Inst[i]->setEnabled(false);
	//}
	
	//Collisions

	//Player and stage Collision
	if(m_pPlayer->isEnabled())
	{
		for(int i=0;i<obstacles;i++)
		{
			if (CollisionManager::CircleAABBTanks(m_pPlayer,m_field[i]))
			{
				float xLess = m_field[i]->getTransform()->position.x - m_pPlayer->getTransform()->position.x;//collision right
				float xLess2 = m_pPlayer->getTransform()->position.x - m_field[i]->getTransform()->position.x;//collision left
				float yLess = m_field[i]->getTransform()->position.y - m_pPlayer->getTransform()->position.y;//collision down
				float yLess2 = m_pPlayer->getTransform()->position.y - m_field[i]->getTransform()->position.y;//collision up
				
				//RightCollision
				if((xLess>xLess2)&&(xLess>yLess)&&(xLess>yLess2))
					m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x - 5.0f;
				//LeftCollision
				else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
					m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 5.0f;
				//DownCollision
				else if ((yLess > xLess) && (yLess >xLess2 ) && (yLess > yLess2))
					m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y - 5.0f;
				//UpCollision
				else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
					m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y + 5.0f;
			}
		}
		for (int i = 0; i < dest; i++)
		{
			if (m_dField[i]->isEnabled())
			{
				if (CollisionManager::CircleAABBTanks(m_pPlayer, m_dField[i]))
				{
					float xLess = m_dField[i]->getTransform()->position.x - m_pPlayer->getTransform()->position.x;//collision right
					float xLess2 = m_pPlayer->getTransform()->position.x - m_dField[i]->getTransform()->position.x;//collision left
					float yLess = m_dField[i]->getTransform()->position.y - m_pPlayer->getTransform()->position.y;//collision down
					float yLess2 = m_pPlayer->getTransform()->position.y - m_dField[i]->getTransform()->position.y;//collision up

					//RightCollision
					if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
						m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x - 5.0f;
					//LeftCollision
					else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
						m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 5.0f;
					//DownCollision
					else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
						m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y - 5.0f;
					//UpCollision
					else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
						m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y + 5.0f;
				}
			}
		}
	}

	//Enemy and stage Collision
	for (int y = 0; y < Enemies; y++)
	{
		if (m_pPlayer->isEnabled())
		{
			for (int i = 0; i < obstacles; i++)
			{
				if (CollisionManager::CircleAABBTanks(m_pEnemy[y], m_field[i]))
				{
					float xLess = m_field[i]->getTransform()->position.x - m_pEnemy[y]->getTransform()->position.x;//collision right
					float xLess2 = m_pEnemy[y]->getTransform()->position.x - m_field[i]->getTransform()->position.x;//collision left
					float yLess = m_field[i]->getTransform()->position.y - m_pEnemy[y]->getTransform()->position.y;//collision down
					float yLess2 = m_pEnemy[y]->getTransform()->position.y - m_field[i]->getTransform()->position.y;//collision up

					//RightCollision
					if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
						m_pEnemy[y]->getTransform()->position.x = m_pEnemy[y]->getTransform()->position.x - 5.0f;
					//LeftCollision
					else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
						m_pEnemy[y]->getTransform()->position.x = m_pEnemy[y]->getTransform()->position.x + 5.0f;
					//DownCollision
					else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
						m_pEnemy[y]->getTransform()->position.y = m_pEnemy[y]->getTransform()->position.y - 5.0f;
					//UpCollision
					else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
						m_pEnemy[y]->getTransform()->position.y = m_pEnemy[y]->getTransform()->position.y + 5.0f;
				}
			}
			for (int i = 0; i < dest; i++)
			{
				if (m_dField[i]->isEnabled())
				{
					if (CollisionManager::CircleAABBTanks(m_pEnemy[y], m_dField[i]))
					{
						float xLess = m_dField[i]->getTransform()->position.x - m_pEnemy[y]->getTransform()->position.x;//collision right
						float xLess2 = m_pEnemy[y]->getTransform()->position.x - m_dField[i]->getTransform()->position.x;//collision left
						float yLess = m_dField[i]->getTransform()->position.y - m_pEnemy[y]->getTransform()->position.y;//collision down
						float yLess2 = m_pEnemy[y]->getTransform()->position.y - m_dField[i]->getTransform()->position.y;//collision up

						//RightCollision
						if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
							m_pEnemy[y]->getTransform()->position.x = m_pEnemy[y]->getTransform()->position.x - 5.0f;
						//LeftCollision
						else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
							m_pEnemy[y]->getTransform()->position.x = m_pEnemy[y]->getTransform()->position.x + 5.0f;
						//DownCollision
						else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
							m_pEnemy[y]->getTransform()->position.y = m_pEnemy[y]->getTransform()->position.y - 5.0f;
						//UpCollision
						else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
							m_pEnemy[y]->getTransform()->position.y = m_pEnemy[y]->getTransform()->position.y + 5.0f;
					}
				}
			}
		}
	}
	
	//Player and enemy Collision
	if (m_pPlayer->isEnabled())
	{
		for (int i = 0; i < Enemies; i++)
		{
			if (CollisionManager::CircleAABBTanks(m_pPlayer, m_pEnemy[i]))
			{
				float xLess = m_pEnemy[i]->getTransform()->position.x - m_pPlayer->getTransform()->position.x;//collision right
				float xLess2 = m_pPlayer->getTransform()->position.x - m_pEnemy[i]->getTransform()->position.x;//collision left
				float yLess = m_pEnemy[i]->getTransform()->position.y - m_pPlayer->getTransform()->position.y;//collision down
				float yLess2 = m_pPlayer->getTransform()->position.y - m_pEnemy[i]->getTransform()->position.y;//collision up

				//RightCollision
				if ((xLess > xLess2) && (xLess > yLess) && (xLess > yLess2))
				{
					m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x - 5.0f;
					m_pEnemy[i]->getTransform()->position.x = m_pEnemy[i]->getTransform()->position.x + 5.0f;
				}
				//LeftCollision
				else if ((xLess2 > xLess) && (xLess2 > yLess) && (xLess2 > yLess2))
				{
					m_pPlayer->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 5.0f;
					m_pEnemy[i]->getTransform()->position.x = m_pEnemy[i]->getTransform()->position.x - 5.0f;
				}
				//DownCollision
				else if ((yLess > xLess) && (yLess > xLess2) && (yLess > yLess2))
				{
					m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y - 5.0f;
					m_pEnemy[i]->getTransform()->position.y = m_pEnemy[i]->getTransform()->position.y + 5.0f;
				}
				//UpCollision
				else if ((yLess2 > xLess) && (yLess2 > xLess2) && (yLess2 > yLess))
				{
					m_pPlayer->getTransform()->position.y = m_pPlayer->getTransform()->position.y + 5.0f;
					m_pEnemy[i]->getTransform()->position.y = m_pEnemy[i]->getTransform()->position.y - 5.0f;
				}
			}
		}
	}

	//PlayerBullets Collision
	{
		//Player bullet and enemy Collision
		for (int i = 0; i < m_pBullet.size(); i++)
		{
			for (int y = 0; y < Enemies; y++)
			{
				if (m_pBullet[i]->isEnabled())
				{
					if (m_pEnemy[y]->isEnabled() == true)
					{
						if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_pEnemy[y]))
						{
							m_pBullet[i]->setEnabled(false);
							int h;
							SoundManager::Instance().playSound("Expl", 0, -1);
							//Damage Enemy0
							if (y == 0)
							{
								h = m_pEnemy[y]->getCurrentHp();
								Enemy0[h - 1]->setEnabled(false);
								SoundManager::Instance().playSound("dmg", 0, -1);
								m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
								if (m_pEnemy[y]->getCurrentHp() == 0)
								{
									m_pEnemy[y]->setEnabled(false);
									m_pEnemy[y]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
									RespawnCD = 0;
									EnemiesDestroyed++;
									SoundManager::Instance().playSound("die", 0, -1);
									if (m_pEnemyDebug[y]->isEnabled())
										m_pEnemyDebug[y]->setEnabled(false);
								}
							}
							//Damage Enemy1
							else if (y == 1)
							{
								m_pEnemy[y]->isHit=true;
								h = m_pEnemy[y]->getCurrentHp();
								Enemy1[h - 1]->setEnabled(false);
								SoundManager::Instance().playSound("dmg", 0, -1);
								m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
								if (m_pEnemy[y]->getCurrentHp() == 0)
								{
									m_pEnemy[y]->setEnabled(false);
									m_pEnemy[y]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
									RespawnCD = 0;
									EnemiesDestroyed++;
									SoundManager::Instance().playSound("die", 0, -1);
									if (m_pEnemyDebug[y]->isEnabled())
										m_pEnemyDebug[y]->setEnabled(false);
								}
							}
							//Damage Enemy2
							else if (y == 2)
							{
								h = m_pEnemy[y]->getCurrentHp();
								Enemy2[h - 1]->setEnabled(false);
								SoundManager::Instance().playSound("dmg", 0, -1);
								m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
								if (m_pEnemy[y]->getCurrentHp() == 0)
								{
									m_pEnemy[y]->setEnabled(false);
									m_pEnemy[y]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
									RespawnCD = 0;
									EnemiesDestroyed++;
									SoundManager::Instance().playSound("die", 0, -1);
									if (m_pEnemyDebug[y]->isEnabled())
										m_pEnemyDebug[y]->setEnabled(false);
								}
							}
							//Damage Enemy3
							else if (y == 3)
							{
								m_pEnemy[y]->isHit = true;
								h = m_pEnemy[y]->getCurrentHp();
								Enemy3[h - 1]->setEnabled(false);
								SoundManager::Instance().playSound("dmg", 0, -1);
								m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
								if (m_pEnemy[y]->getCurrentHp() == 0)
								{
									m_pEnemy[y]->setEnabled(false);
									m_pEnemy[y]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
									RespawnCD = 0;
									EnemiesDestroyed++;
									SoundManager::Instance().playSound("die", 0, -1);
									if (m_pEnemyDebug[y]->isEnabled())
										m_pEnemyDebug[y]->setEnabled(false);
								}
							}
						}
					}

				}
			}
			for (int y = 0; y < dest; y++)
			{
				if (m_pBullet[i]->isEnabled())
				{
					if (m_dField[y]->isEnabled())
					{
						if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_dField[y]))
						{
							m_pBullet[i]->setEnabled(false);
							int h = 0;
							SoundManager::Instance().playSound("Expl", 0, -1);
							//Damage Tree right
							if (y == 0)
							{
								h = m_dField[y]->getCurrentHp();
								Tree1[h - 1]->setEnabled(false);
								m_dField[y]->setCurrentHp(m_dField[y]->getCurrentHp() - 1);
								if (m_dField[y]->getCurrentHp() == 0)
								{
									for (auto node : m_pRightTreeNodes)
									{
										m_pSGrid.push_back(node);
									}
									m_dField[y]->setEnabled(false);
								}
							}
							//Damage Tree left
							else if (y == 1)
							{
								h = m_dField[y]->getCurrentHp();
								Tree2[h - 1]->setEnabled(false);
								m_dField[y]->setCurrentHp(m_dField[y]->getCurrentHp() - 1);
								if (m_dField[y]->getCurrentHp() == 0)
								{
									for (auto node : m_pLeftTreeNodes)
									{
										m_pSGrid.push_back(node);
									}
									m_dField[y]->setEnabled(false);
								}
							}

						}
					}
				}
			}
		}

		//Player bullet and Stage collision
		for (int i = 0; i < m_pBullet.size(); i++)
		{
			for (int y = 0; y < obstacles; y++)
			{
				if (m_pBullet[i]->isEnabled())
				{
					if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_field[y]))
					{
						m_pBullet[i]->setEnabled(false);
						SoundManager::Instance().playSound("Expl", 0, -1);
					}
				}
			}

		}
	}

	//Enemy BulletCollision
	{
		//Enemy Bullet and player Collision
		if (m_pPlayer->isEnabled() == true)
		{
			for (int i = 0; i < m_pEnemyBullet.size(); i++)
			{
				if (m_pEnemyBullet[i]->isEnabled())
				{
					if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_pPlayer))
					{
						m_pEnemyBullet[i]->setEnabled(false);
						SoundManager::Instance().playSound("Expl", 0, -1);
						m_pEnemy[i]->AttackCd = 0;
						int h = 0;
						//Damage Player
						h = m_pPlayer->getCurrentHp();
						PlayerHp[h - 1]->setEnabled(false);
						m_pPlayer->setCurrentHp(m_pPlayer->getCurrentHp() - 1);
						if (m_pPlayer->getCurrentHp() == 0)
						{
							m_pPlayer->setEnabled(false);
							SoundManager::Instance().playSound("die", 0, -1);
						}
					}
				}
			}
		}
		//Enemy Bullet and Destruction Collision
		for (int i = 0; i < m_pEnemyBullet.size(); i++)
		{
			for (int y = 0; y < dest; y++)
			{
				if (m_pEnemyBullet[i]->isEnabled())
				{
					if (m_dField[y]->isEnabled())
					{
						if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_dField[y]))
						{
							m_pEnemyBullet[i]->setEnabled(false);
							int h = 0;
							SoundManager::Instance().playSound("Expl", 0, -1);
							//Damage Tree right
							if (y == 0)
							{
								h = m_dField[y]->getCurrentHp();
								Tree1[h - 1]->setEnabled(false);
								m_dField[y]->setCurrentHp(m_dField[y]->getCurrentHp() - 1);
								if (m_dField[y]->getCurrentHp() == 0)
								{
									for (auto node : m_pRightTreeNodes)
									{
										m_pSGrid.push_back(node);
									}
									m_dField[y]->setEnabled(false);
								}
							}
							//Damage Tree left
							else if (y == 1)
							{
								h = m_dField[y]->getCurrentHp();
								Tree2[h - 1]->setEnabled(false);
								m_dField[y]->setCurrentHp(m_dField[y]->getCurrentHp() - 1);
								if (m_dField[y]->getCurrentHp() == 0)
								{
									for (auto node : m_pLeftTreeNodes)
									{
										m_pSGrid.push_back(node);
									}
									m_dField[y]->setEnabled(false);
								}
							}

						}
					}
				}
			}
		}
		//Enemy Bullet and stage Collision
		if (m_pPlayer->isEnabled())
		{
			for (int i = 0; i < m_pEnemyBullet.size(); i++)
			{
				if (m_pEnemyBullet[i]->isEnabled() == true)
				{
					for (int y = 0; y < obstacles; y++)
					{
						if (m_pEnemyBullet[i]->isEnabled())
						{
							if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_field[y]))
							{
								m_pEnemyBullet[i]->setEnabled(false);
								SoundManager::Instance().playSound("Expl", 0, -1);
							}
						}
					}
				}
			}
		}
		
	}

	//Respawn
	for (int i = 0; i < Enemies; i++)
	{
		if (m_pEnemy[i]->getTransform()->position.x < 0 || m_pEnemy[i]->getTransform()->position.x > 800 ||
			m_pEnemy[i]->getTransform()->position.y < 0 || m_pEnemy[i]->getTransform()->position.y > 600)
		{
			//if (fleed == false)
			//{
			//	RespawnCD = 0;
			//	fleed = true;
			//}
			m_pEnemy[i]->p0 = false;
			m_pEnemy[i]->p1 = false;
			m_pEnemy[i]->p2 = false;
			m_pEnemy[i]->p3 = false;
			m_pEnemy[i]->p4 = false;

			m_pEnemy[i]->getTransform()->position = glm::vec2({ -200.0f,-200.0f });
			m_pEnemy[i]->move = false;
			m_pEnemy[i]->active = false;
			m_pEnemy[i]->setEnabled(true);
			m_pEnemy[i]->setCurrentHp(4);
			if (m_pEnemy[i]->leftEnemy == true)
			{
				m_pEnemy[i]->leftEnemy = false;
				leftEnemyActive = false;
			}
			else if (m_pEnemy[i]->rightEnemy == true)
			{
				m_pEnemy[i]->rightEnemy = false;
				rightEnemyActive = false;
			}
			if (i == 0)
			{
				Enemy0[0]->setEnabled(true);
				Enemy0[1]->setEnabled(true);
				Enemy0[2]->setEnabled(true);
				Enemy0[3]->setEnabled(true);
			}
			else if (i == 1)
			{
				Enemy1[0]->setEnabled(true);
				Enemy1[1]->setEnabled(true);
				Enemy1[2]->setEnabled(true);
				Enemy1[3]->setEnabled(true);
			}
			else if (i == 2)
			{
				Enemy2[0]->setEnabled(true);
				Enemy2[1]->setEnabled(true);
				Enemy2[2]->setEnabled(true);
				Enemy2[3]->setEnabled(true);
			}
			else if (i == 3)
			{
				Enemy3[0]->setEnabled(true);
				Enemy3[1]->setEnabled(true);
				Enemy3[2]->setEnabled(true);
				Enemy3[3]->setEnabled(true);
			}
		}
	}
	if (RespawnCD >= 8)
	{
		auto offsetTiles1 = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
		int r = rand() % 2;
		//Close Combat
		if (r == 0)
		{
			if (leftEnemyActive == false)
			{
				if (m_pEnemy[0]->active == true)
				{
					m_pEnemy[2]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[2]->move = true;
					m_pEnemy[2]->active = true;
					m_pEnemy[2]->leftEnemy = true;
					m_pEnemy[2]->flee = false;
					m_p2CloseCombatStateMachine->setCurrentState(CloseCombat2PatrolState);
					leftEnemyActive = true;
					fleed = false;
				}
				else
				{
					m_pEnemy[0]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[0]->move = true;
					m_pEnemy[0]->active = true;
					m_pEnemy[0]->leftEnemy = true;
					m_pEnemy[0]->flee = false;
					m_pCloseCombatStateMachine->setCurrentState(CloseCombatPatrolState);
					leftEnemyActive = true;
					fleed = false;
				}
			}
			if (rightEnemyActive == false)
			{
				if (m_pEnemy[0]->active == true)
				{
					m_pEnemy[2]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[2]->move = true;
					m_pEnemy[2]->active = true;
					m_pEnemy[2]->rightEnemy = true;
					m_pEnemy[2]->flee = false;
					m_p2CloseCombatStateMachine->setCurrentState(CloseCombat2PatrolState);
					rightEnemyActive = true;
					fleed = false;
				}
				else
				{
					m_pEnemy[0]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[0]->move = true;
					m_pEnemy[0]->active = true;
					m_pEnemy[0]->rightEnemy = true;
					m_pEnemy[0]->flee = false;
					m_pCloseCombatStateMachine->setCurrentState(CloseCombatPatrolState);
					rightEnemyActive = true;
					fleed = false;
				}
			}
		}
		else if (r == 1)
		{
			//Ranged
			if (leftEnemyActive == false)
			{
				if (m_pEnemy[1]->active == true)
				{
					m_pEnemy[3]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;;
					m_pEnemy[3]->move = true;
					m_pEnemy[3]->active = true;
					m_pEnemy[3]->leftEnemy = true;
					m_pEnemy[3]->flee = false;
					m_p2RangedStateMachine->setCurrentState(Ranged2PatrolState);
					leftEnemyActive = true;
					fleed = false;
				}
				else
				{
					m_pEnemy[1]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[1]->move = true;
					m_pEnemy[1]->active = true;
					m_pEnemy[1]->leftEnemy = true;
					m_pEnemy[1]->flee = false;
					m_pRangedStateMachine->setCurrentState(RangedPatrolState);
					leftEnemyActive = true;
					fleed = false;
				}
			}
			else if (rightEnemyActive == false)
			{
				if (m_pEnemy[1]->active == true)
				{
					m_pEnemy[3]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[3]->move = true;
					m_pEnemy[3]->active = true;
					m_pEnemy[3]->rightEnemy = true;
					m_pEnemy[3]->flee = false;
					m_p2RangedStateMachine->setCurrentState(Ranged2PatrolState);
					rightEnemyActive = true;
					fleed = false;
				}
				else
				{
					m_pEnemy[1]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
					m_pEnemy[1]->move = true;
					m_pEnemy[1]->active = true;
					m_pEnemy[1]->rightEnemy = true;
					m_pEnemy[1]->flee = false;
					m_pRangedStateMachine->setCurrentState(RangedPatrolState);
					rightEnemyActive = true;
					fleed = false;
				}
			}
		}
	}
	//Avoidance TODO
	for (int i = 0; i < Enemies; i++)
	{
		if(m_pEnemy[i]->patrol==false)
		{
			for (int y = 0; y < obstacles; y++)
			{
				//Left whishker
				if (CollisionManager::lineRectCheck(m_pEnemy[i]->m_LWhishker.Start(),
					m_pEnemy[i]->m_LWhishker.End(), m_field[y]->getTransform()->position-
					glm::vec2(m_field[y]->getWidth()/2, m_field[y]->getHeight()/2),
					m_field[y]->getWidth(), m_field[y]->getHeight()))
				{
					m_pEnemy[i]->ColObsL = y;
					m_pEnemy[i]->tRight = true;
					m_pEnemy[i]->avoidance = true;
				}
				if(m_pEnemy[i]->ColObsL!=100)
				{
					if (!(CollisionManager::lineRectCheck(m_pEnemy[i]->m_LWhishker.Start(),
						m_pEnemy[i]->m_LWhishker.End(), m_field[m_pEnemy[i]->ColObsL]->getTransform()->position -
						glm::vec2(m_field[m_pEnemy[i]->ColObsL]->getWidth() / 2, m_field[m_pEnemy[i]->ColObsL]->getHeight() / 2),
						m_field[m_pEnemy[i]->ColObsL]->getWidth(), m_field[m_pEnemy[i]->ColObsL]->getHeight())))
					{
						m_pEnemy[i]->tRight = false;
						m_pEnemy[i]->avocd = 0;
						if(m_pEnemy[i]->avocd>1)
						{
							m_pEnemy[i]->avoidance = false;
							m_pEnemy[i]->ColObsL = 100;
						}
					}
				}
				//Right Whishker
				if (CollisionManager::lineRectCheck(m_pEnemy[i]->m_RWhishker.Start(),
					m_pEnemy[i]->m_RWhishker.End(), m_field[y]->getTransform()->position -
					glm::vec2(m_field[y]->getWidth() / 2, m_field[y]->getHeight() / 2),
					m_field[y]->getWidth(), m_field[y]->getHeight()))
				{
					m_pEnemy[i]->ColObsR = y;
					m_pEnemy[i]->avoidance = true;
					m_pEnemy[i]->tLeft = true;
				}
				if (m_pEnemy[i]->ColObsR != 100)
				{
					if (!(CollisionManager::lineRectCheck(m_pEnemy[i]->m_RWhishker.Start(),
						m_pEnemy[i]->m_RWhishker.End(), m_field[m_pEnemy[i]->ColObsR]->getTransform()->position -
						glm::vec2(m_field[m_pEnemy[i]->ColObsR]->getWidth() / 2, m_field[m_pEnemy[i]->ColObsR]->getHeight() / 2),
						m_field[m_pEnemy[i]->ColObsR]->getWidth(), m_field[m_pEnemy[i]->ColObsR]->getHeight())))
					{
						m_pEnemy[i]->tLeft = false;
						m_pEnemy[i]->avocd = 0;
						if (m_pEnemy[i]->avocd > 1)
						{
							m_pEnemy[i]->avoidance = false;
							m_pEnemy[i]->ColObsL = 100;
						}
					}
				}
			}
		}
	}

		////Win Condition
	if (m_pPlayer->isEnabled() == false)
	{
		if (ButtonCD > 1)
		{
			TheGame::Instance()->changeSceneState(LOSE_SCENE);
		}
	}
	else if ((leftEnemyActive == false) && (rightEnemyActive == false))
	{
		if(fleed==false)
		TheGame::Instance()->changeSceneState(WIN_SCENE);
	}
}
	

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	//Debug
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
	if(ButtonCD>1)
	{
		if (Debug)
			Debug = false;
		else
		{
			Debug = true;
		}
		for (int i = 0; i < Enemies; i++)
		{
			if (m_pEnemy[i]->isEnabled())
			m_pEnemyDebug[i]->setEnabled(Debug);

			else
			m_pEnemyDebug[i]->setEnabled(false);
		}

		ButtonCD = 0;
	}
	m_setGridEnabled(Debug);
	m_toggleGrid(Debug);
	}
	//Damage Test
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K)) 
	{
		if(Debug==true)
		{
			if (ButtonCD > 1)
			{
				for (int i = 0; i < Enemies; i++)
				{

					int h;
					//Damage Enemy Left
					if (i == 0)
					{
						h = m_pEnemy[i]->getCurrentHp();
						Enemy0[h - 1]->setEnabled(false);
						SoundManager::Instance().playSound("dmg", 0, -1);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);

							EnemiesDestroyed++;
							SoundManager::Instance().playSound("die", 0, -1);
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
					//Damage Enemy Right
					else if (i  == 1)
					{
						h = m_pEnemy[i]->getCurrentHp();
						Enemy1[h - 1]->setEnabled(false);
						SoundManager::Instance().playSound("dmg", 0, -1);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							SoundManager::Instance().playSound("die", 0, -1);
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
				}
				ButtonCD = 0;
			}
		}
	}
	//Pause
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
	{
		if (Debug == true)
		{
			if (ButtonCD > 1)
			{
				for (int i = 0; i < Enemies; i++)
				{

					if(m_pEnemy[i]->move==true)
					m_pEnemy[i]->move = false;

					else
						m_pEnemy[i]->move = true;
				}
				ButtonCD = 0;
			}
		}
	}

	//Player Attacks
	{
		//Player CloseCombat CloseCombatAttack
		if (EventManager::Instance().getMouseButton(0) && GunCD > 1)
		{
			if (m_pPlayer->isEnabled() == true)
			{
				SoundManager::Instance().playSound("at", 0, -1);
				for (int i = 0; i < Enemies; i++)
				{
					if (m_pEnemy[i]->isEnabled())
					{
						m_CheckCloseCombat(m_pPlayer,m_pEnemy[i]);
						if (m_pPlayer->isInCloseCombatDistance())
						{
							if (CollisionManager::lineRectCheck(m_pPlayer->getTransform()->position,
								m_pPlayer->getTransform()->position + m_pPlayer->getOrientation() * m_pPlayer->getCloseCombatDistance(),
								m_pEnemy[i]->getTransform()->position, m_pEnemy[i]->getWidth(), m_pEnemy[i]->getHeight()))
							{
								GunCD = 0;
								int h = 0;
								//Damage Enemy 0
								if (i == 0)
								{
									h = m_pEnemy[i]->getCurrentHp();
									Enemy0[h - 1]->setEnabled(false);
									SoundManager::Instance().playSound("dmg", 0, -1);
									m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
									if (m_pEnemy[i]->getCurrentHp() == 0)
									{
										m_pEnemy[i]->setEnabled(false);
										m_pEnemy[i]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
										RespawnCD = 0;
										EnemiesDestroyed++;
										SoundManager::Instance().playSound("die", 0, -1);
										if (m_pEnemyDebug[i]->isEnabled())
											m_pEnemyDebug[i]->setEnabled(false);
									}
								}
								//Damage Enemy 1
								else if (i == 1)
								{
									m_pEnemy[i]->isHit=true;
									h = m_pEnemy[i]->getCurrentHp();
									Enemy1[h - 1]->setEnabled(false);
									SoundManager::Instance().playSound("dmg", 0, -1);
									m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
									if (m_pEnemy[i]->getCurrentHp() == 0)
									{
										m_pEnemy[i]->setEnabled(false);
										m_pEnemy[i]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
										RespawnCD = 0;
										EnemiesDestroyed++;
										SoundManager::Instance().playSound("die", 0, -1);
										if (m_pEnemyDebug[i]->isEnabled())
											m_pEnemyDebug[i]->setEnabled(false);
									}
								}
								//Damage Enemy 2
								else if (i == 2)
								{
									h = m_pEnemy[i]->getCurrentHp();
									Enemy2[h - 1]->setEnabled(false);
									SoundManager::Instance().playSound("dmg", 0, -1);
									m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
									if (m_pEnemy[i]->getCurrentHp() == 0)
									{
										m_pEnemy[i]->setEnabled(false);
										m_pEnemy[i]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
										RespawnCD = 0;
										EnemiesDestroyed++;
										SoundManager::Instance().playSound("die", 0, -1);
										if (m_pEnemyDebug[i]->isEnabled())
											m_pEnemyDebug[i]->setEnabled(false);
									}
								}
								//Damage Enemy 3
								else if (i == 3)
								{
									m_pEnemy[i]->isHit = true;
									h = m_pEnemy[i]->getCurrentHp();
									Enemy3[h - 1]->setEnabled(false);
									SoundManager::Instance().playSound("dmg", 0, -1);
									m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
									if (m_pEnemy[i]->getCurrentHp() == 0)
									{
										m_pEnemy[i]->setEnabled(false);
										m_pEnemy[i]->getTransform()->position = glm::vec2{ -100.0f,-100.0f };
										RespawnCD = 0;
										EnemiesDestroyed++;
										SoundManager::Instance().playSound("die", 0, -1);
										if (m_pEnemyDebug[i]->isEnabled())
											m_pEnemyDebug[i]->setEnabled(false);
									}
								}
							}
						}
					}
				}

				for (int i = 0; i < dest; i++)
				{
					if (m_dField[i]->isEnabled())
					{
						m_CheckCloseCombat(m_pPlayer,m_dField[i]);
						if (m_pPlayer->isInCloseCombatDistance())
						{
							if (CollisionManager::lineRectCheck(m_pPlayer->getTransform()->position,
								m_pPlayer->getTransform()->position + m_pPlayer->getOrientation() * m_pPlayer->getCloseCombatDistance(),
								m_dField[i]->getTransform()->position, m_dField[i]->getWidth(), m_dField[i]->getHeight()))
							{
								GunCD = 0;
								int h = 0;
								//Damage Tree Right
								if (i == 0)
								{
									h = m_dField[i]->getCurrentHp();
									Tree1[h - 1]->setEnabled(false);
									m_dField[i]->setCurrentHp(m_dField[i]->getCurrentHp() - 1);
									if (m_dField[i]->getCurrentHp() == 0)
									{
										for (auto node : m_pRightTreeNodes)
										{
											m_pSGrid.push_back(node);
										}
										m_dField[i]->setEnabled(false);
									}
								}
								//Damage Tree Left
								else if (i == 1)
								{
									h = m_dField[i]->getCurrentHp();
									Tree2[h - 1]->setEnabled(false);
									m_dField[i]->setCurrentHp(m_dField[i]->getCurrentHp() - 1);
									if (m_dField[i]->getCurrentHp() == 0)
									{
										for (auto node : m_pLeftTreeNodes)
										{
											m_pSGrid.push_back(node);
										}
										m_dField[i]->setEnabled(false);
									}
								}
							}
						}
					}
				}

				GunCD = 0;
			}
		}

		//Player BulletShooting
		if (EventManager::Instance().getMouseButton(2) && GunCD > 1)
		{
			if (m_pPlayer->isEnabled() == true)
			{
				GunCD = 0;
				m_pBullet.push_back(new Bullet(m_pPlayer->getRotation(), m_pPlayer->getTransform()->position, 
					"../Assets/textures/Tbullet.png", "Tbullet", true));
				addChild(m_pBullet[TotalBullets]);
				TotalBullets++;
				SoundManager::Instance().playSound("sht", 0, -1);
			}
		}
	}
	
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	//Offsets
		auto offsetTiles1 = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
		auto offsetTiles2 = glm::vec2(Config::TILE_SIZE * 1.0f, Config::TILE_SIZE * 0.5f);
		auto offsetEnemiesDown = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f + 60.0f);
		auto offsetEnemiesUp = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f - 60.0f);
		auto offsetEnemiesRight = glm::vec2(Config::TILE_SIZE * 0.5f + 60.0f, Config::TILE_SIZE * 0.5f);
		auto offsetEnemiesLeft = glm::vec2(Config::TILE_SIZE * 0.5f - 60.0f, Config::TILE_SIZE * 0.5f);
		
	//Labels
	{
		const SDL_Color blue = { 0, 0, 255, 255 };
		m_Inst[0] = new Label("Remainding Slimes: 6.          Slimes Killed: 0.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[0]->setEnabled(false);
		m_Inst[0]->setParent(this);
		addChild(m_Inst[0], 4);

		m_Inst[1] = new Label("Remainding Slimes: 5.          Slimes Killed: 1.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[1]->setEnabled(false);
		m_Inst[1]->setParent(this);
		addChild(m_Inst[1], 4);

		m_Inst[2] = new Label("Remainding Slimes: 4.          Slimes Killed: 2.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[2]->setEnabled(false);
		m_Inst[2]->setParent(this);
		addChild(m_Inst[2], 4);

		m_Inst[3] = new Label("Remainding Slimes: 3.          Slimes Killed: 3.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[3]->setEnabled(false);
		m_Inst[3]->setParent(this);
		addChild(m_Inst[3], 4);

		m_Inst[4] = new Label("Remainding Slimes: 2.          Slimes Killed: 4.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[4]->setEnabled(false);
		m_Inst[4]->setParent(this);
		addChild(m_Inst[4], 4);

		m_Inst[5] = new Label("Remainding Slimes: 1.          Slimes Killed: 5.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[5]->setEnabled(false);
		m_Inst[5]->setParent(this);
		addChild(m_Inst[5], 4);

		m_Inst[6] = new Label("Remainding Slimes: 0.          Slimes Killed: 6.", "Consolas",
			20, blue, glm::vec2(400.f, 550.f));
		m_Inst[6]->setEnabled(false);
		m_Inst[6]->setParent(this);
		addChild(m_Inst[6], 4);
	}

	//Tiles
	m_buildGrid();
	m_buildGridSight();

	//Background
	Bg = new TileC("../Assets/grid/Bg.png", "Bg");
	Bg->getTransform()->position.x = 800.0f/2;		
	Bg->getTransform()->position.y = 600.0f/2;
	addChild(Bg,0);

	//Obstacles
	{
		m_field[0] = new TileC("../Assets/grid/River120.png", "120");
		m_field[0]->getTransform()->position = m_getTile(1, 5)->getTransform()->position + offsetTiles1;
		addChild(m_field[0], 1);
		m_pMap.push_back(m_field[0]);

		m_field[1] = new TileC("../Assets/grid/River120.png", "120");
		m_field[1]->getTransform()->position = m_getTile(4, 5)->getTransform()->position + offsetTiles1;
		addChild(m_field[1], 1);
		m_pMap.push_back(m_field[1]);

		m_field[2] = new TileC("../Assets/grid/River120.png", "120");
		m_field[2]->getTransform()->position = m_getTile(7, 5)->getTransform()->position + offsetTiles1;
		addChild(m_field[2], 1);
		m_pMap.push_back(m_field[2]);

		m_field[3] = new TileC("/Assets/grid/River120.png", "120");
		m_field[3]->getTransform()->position = m_getTile(12, 5)->getTransform()->position + offsetTiles1;
		addChild(m_field[3], 2);
		m_pMap.push_back(m_field[3]);

		m_field[4] = new TileC("../Assets/grid/River120.png", "120");
		m_field[4]->getTransform()->position = m_getTile(15, 5)->getTransform()->position + offsetTiles1;
		addChild(m_field[4], 1);
		m_pMap.push_back(m_field[4]);

		m_field[5] = new TileC("../Assets/grid/River120.png", "120");
		m_field[5]->getTransform()->position = m_getTile(18, 5)->getTransform()->position + offsetTiles1;
		addChild(m_field[5], 1);
		m_pMap.push_back(m_field[5]);
	}

	//Destructibles
	{
		m_dField[0] = new DestructibleObstacle(3, "../Assets/grid/gridTree.png", "TreeG");
		m_dField[0]->getTransform()->position = m_getTile(14, 10)->getTransform()->position + offsetTiles1;
		addChild(m_dField[0], 1);
		m_pMap.push_back(m_dField[0]);
		//Hp Tree1
		Tree1[0] = new Hp();
		Tree1[0]->getTransform()->position = { m_dField[0]->getTransform()->position.x,m_dField[0]->getTransform()->position.y - 40 };
		addChild(Tree1[0], 3);
		Tree1[1] = new Hp();
		Tree1[1]->getTransform()->position = { m_dField[0]->getTransform()->position.x + 10,m_dField[0]->getTransform()->position.y - 40 };
		addChild(Tree1[1], 3);
		Tree1[2] = new Hp();
		Tree1[2]->getTransform()->position = { m_dField[0]->getTransform()->position.x - 10,m_dField[0]->getTransform()->position.y - 40 };
		addChild(Tree1[2], 3);

		m_dField[1] = new DestructibleObstacle(3, "../Assets/grid/gridTree.png", "TreeG");
		m_dField[1]->getTransform()->position = m_getTile(5, 10)->getTransform()->position + offsetTiles1;
		addChild(m_dField[1], 2);
		m_pMap.push_back(m_dField[1]);
		//Hp Tree2
		Tree2[0] = new Hp();
		Tree2[0]->getTransform()->position = { m_dField[1]->getTransform()->position.x,m_dField[1]->getTransform()->position.y - 40 };
		addChild(Tree2[0], 3);
		Tree2[1] = new Hp();
		Tree2[1]->getTransform()->position = { m_dField[1]->getTransform()->position.x + 10,m_dField[1]->getTransform()->position.y - 40 };
		addChild(Tree2[1], 3);
		Tree2[2] = new Hp();
		Tree2[2]->getTransform()->position = { m_dField[1]->getTransform()->position.x - 10,m_dField[1]->getTransform()->position.y - 40 };
		addChild(Tree2[2], 3);
	}
		
	//ENEMIES
	{
		//Enemy0
		m_pEnemy[0] = new CloseCombatEnemy("../Assets/textures/Slime.png", "sl");
		m_pEnemy[0]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
		m_pEnemy[0]->active = true;
		m_pEnemy[0]->rightEnemy = true;
		addChild(m_pEnemy[0], 2);
		//Hp
		m_pEnemy[0]->setCurrentHp(4);
		Enemy0[0] = new Hp();
		Enemy0[0]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x,m_pEnemy[0]->getTransform()->position.y - 40 };
		addChild(Enemy0[0], 3);
		Enemy0[1] = new Hp();
		Enemy0[1]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x + 10,m_pEnemy[0]->getTransform()->position.y - 40 };
		addChild(Enemy0[1], 3);
		Enemy0[2] = new Hp();
		Enemy0[2]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x - 10,m_pEnemy[0]->getTransform()->position.y - 40 };
		addChild(Enemy0[2], 3);
		Enemy0[3] = new Hp();
		Enemy0[3]->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x - 20,m_pEnemy[0]->getTransform()->position.y - 40 };
		addChild(Enemy0[3], 3);

		//Enemy1
		m_pEnemy[1] = new RangedCombatEnemy("../Assets/textures/Slime2.png", "sl2");
		m_pEnemy[1]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
		m_pEnemy[1]->active = true;
		m_pEnemy[1]->leftEnemy = true;
		addChild(m_pEnemy[1], 2);
		//Hp
		m_pEnemy[1]->setCurrentHp(4);
		Enemy1[0] = new Hp();
		Enemy1[0]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x,m_pEnemy[1]->getTransform()->position.y - 40 };
		addChild(Enemy1[0], 3);
		Enemy1[1] = new Hp();
		Enemy1[1]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x + 10,m_pEnemy[1]->getTransform()->position.y - 40 };
		addChild(Enemy1[1], 3);
		Enemy1[2] = new Hp();
		Enemy1[2]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x - 10,m_pEnemy[1]->getTransform()->position.y - 40 };
		addChild(Enemy1[2], 3);
		Enemy1[3] = new Hp();
		Enemy1[3]->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x - 20,m_pEnemy[1]->getTransform()->position.y - 40 };
		addChild(Enemy1[3], 3);

		//Enemy2
		m_pEnemy[2] = new CloseCombatEnemy("../Assets/textures/Slime.png", "sl");
		m_pEnemy[2]->getTransform()->position = glm::vec2({ -200,-200 });/*m_getTile(0, 5)->getTransform()->position + offsetEnemiesLeft;*/
		addChild(m_pEnemy[2], 2);
		//Hp
		m_pEnemy[2]->setCurrentHp(4);
		Enemy2[0] = new Hp();
		Enemy2[0]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x,m_pEnemy[2]->getTransform()->position.y - 40 };
		addChild(Enemy2[0], 3);							 
		Enemy2[1] = new Hp();							 
		Enemy2[1]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x + 10,m_pEnemy[2]->getTransform()->position.y - 40 };
		addChild(Enemy2[1], 3);							 
		Enemy2[2] = new Hp();							 
		Enemy2[2]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x - 10,m_pEnemy[2]->getTransform()->position.y - 40 };
		addChild(Enemy2[2], 3);							
		Enemy2[3] = new Hp();							 
		Enemy2[3]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x - 20,m_pEnemy[2]->getTransform()->position.y - 40 };
		addChild(Enemy2[3], 3);

		//Enemy3
		m_pEnemy[3] = new RangedCombatEnemy("../Assets/textures/Slime2.png", "sl2");
		m_pEnemy[3]->getTransform()->position = glm::vec2({ -200,-200 });//m_getTile(19, 5)->getTransform()->position + offsetEnemiesRight;
		addChild(m_pEnemy[3], 2);
		//Hp
		m_pEnemy[3]->setCurrentHp(4);
		Enemy3[0] = new Hp();
		Enemy3[0]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x,m_pEnemy[3]->getTransform()->position.y - 40 };
		addChild(Enemy3[0], 3);
		Enemy3[1] = new Hp();
		Enemy3[1]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x + 10,m_pEnemy[3]->getTransform()->position.y - 40 };
		addChild(Enemy3[1], 3);
		Enemy3[2] = new Hp();
		Enemy3[2]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x - 10,m_pEnemy[3]->getTransform()->position.y - 40 };
		addChild(Enemy3[2], 3);
		Enemy3[3] = new Hp();
		Enemy3[3]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x - 20,m_pEnemy[3]->getTransform()->position.y - 40 };
		addChild(Enemy3[3], 3);

		//Enemy Debug//
		for (int i = 0; i < Enemies; i++)
		{
			m_pEnemyDebug[i] = new EnemyDebugMode(m_pEnemy[i]);
			m_pEnemyDebug[i]->setEnabled(false);
			addChild(m_pEnemyDebug[i], 0);
		}
	}

	//PLAYER:
	{
		//PlayerAgent
		m_pPlayer = new PlayerAgent();
		m_pPlayer->getTransform()->position = m_getTile(10, 0)->getTransform()->position + offsetTiles1;
		m_pPlayer->setEnabled(true);
		addChild(m_pPlayer, 2);
		m_pMap.push_back(m_pPlayer);

		//Player HP
		m_pPlayer->setCurrentHp(5);
		PlayerHp[0] = new Hp();
		PlayerHp[0]->getTransform()->position = { m_pPlayer->getTransform()->position.x,m_pPlayer->getTransform()->position.y - 40 };
		addChild(PlayerHp[0], 3);
		PlayerHp[1] = new Hp();
		PlayerHp[1]->getTransform()->position = { m_pPlayer->getTransform()->position.x + 10,m_pPlayer->getTransform()->position.y - 40 };
		addChild(PlayerHp[1], 3);
		PlayerHp[2] = new Hp();
		PlayerHp[2]->getTransform()->position = { m_pPlayer->getTransform()->position.x - 10,m_pPlayer->getTransform()->position.y - 40 };
		addChild(PlayerHp[2], 3);
		PlayerHp[3] = new Hp();
		PlayerHp[3]->getTransform()->position = { m_pPlayer->getTransform()->position.x - 20,m_pPlayer->getTransform()->position.y - 40 };
		addChild(PlayerHp[3], 3);
		PlayerHp[4] = new Hp();
		PlayerHp[4]->getTransform()->position = { m_pPlayer->getTransform()->position.x + 20,m_pPlayer->getTransform()->position.y - 40 };
		addChild(PlayerHp[4], 3);
	}

	//StateMachine
	m_buildCloseCombatStateMachine();
	m_buildCloseCombatStateMachine2();
	m_buildRangedStateMachine();
	m_buildRangedStateMachine2();

	
	////DECISION TREES:
	////Enemy 0
	//for (int i = 0; i < Enemies; i++)
	//{
	//	decisionTree[i] = new DecisionTree();
	//	decisionTree[i]->setAgent(m_pEnemy[i]);
	//}
}

void PlayScene::GUI_Function() const
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("GAME3001 - Lab 3", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	static bool isGridEnabled = false;
	if (ImGui::Checkbox("Grid Enabled", &isGridEnabled))
	{
		//turn the grid on/off
		m_setGridEnabled(isGridEnabled);
	}

	//ImGui::Separator();
	//
	//auto radio = static_cast<int>(currentHeuristic);
	//ImGui::LabelText("", "Heuristic Type");
	//ImGui::RadioButton("Euclidean", &radio, static_cast<int>(EUCLIDEAN));
	//ImGui::SameLine();
	//ImGui::RadioButton("Manhattan", &radio, static_cast<int>(MANHATTAN));
	//
	//if (currentHeuristic != Heuristic(radio))
	//{
	//	currentHeuristic = Heuristic(radio);
	//	m_computeTileCost();
	//}
	//
	//ImGui::Separator();
	//
	//static int startPosition[] = { m_pShip->getGridPosition().x,m_pShip->getGridPosition().y };
	//if (ImGui::SliderInt2("Start Position", startPosition, 0, Config::COL_NUM - 1))
	//{
	//	//Row adjustment so that it does not go out screen
	//	if (startPosition[1] > Config::ROW_NUM - 1)
	//	{
	//		startPosition[1] = Config::ROW_NUM - 1;
	//	}
	//	SDL_RenderClear(Renderer::Instance()->getRenderer());
	//	m_getTile(m_pShip->getGridPosition())->setTileStatus(UNVISITED);
	//	m_pShip->getTransform()->position = m_getTile(startPosition[0], startPosition[1])->getTransform()->position + offset;
	//	m_pShip->setGridPosition(startPosition[0], startPosition[1]);
	//	m_getTile(m_pShip->getGridPosition())->setTileStatus(START);
	//	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	//	SDL_RenderPresent(Renderer::Instance()->getRenderer());
	//}
	//
	//static int targetPosition[] = { m_pTarget->getGridPosition().x,m_pTarget->getGridPosition().y };
	//if (ImGui::SliderInt2("Target Position", targetPosition, 0, Config::COL_NUM - 1))
	//{
	//	//Row adjustment so that it does not go out screen
	//	if (targetPosition[1] > Config::ROW_NUM - 1)
	//	{
	//		targetPosition[1] = Config::ROW_NUM - 1;
	//	}
	//	SDL_RenderClear(Renderer::Instance()->getRenderer());
	//	m_getTile(m_pTarget->getGridPosition())->setTileStatus(UNVISITED);
	//	m_pTarget->getTransform()->position = m_getTile(targetPosition[0], targetPosition[1])->getTransform()->position + offset;
	//	m_pTarget->setGridPosition(targetPosition[0], targetPosition[1]);
	//	m_getTile(m_pTarget->getGridPosition())->setTileStatus(GOAL);
	//	m_computeTileCost();
	//	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	//	SDL_RenderPresent(Renderer::Instance()->getRenderer());
	//}
	//ImGui::Separator();
	//if (ImGui::Button("Start"))
	//{
	//	isMoving = true;
	//}
	//
	//ImGui::SameLine();
	//
	//if (ImGui::Button("Reset"))
	//{
	//	m_reset();
	//}
	//
	//ImGui::Separator();

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::m_buildGrid()
{

	auto tileSize = Config::TILE_SIZE;
		//Add tiles to the grid
		for (int row = 0; row < Config::ROW_NUM; ++row)
		{
			for (int col = 0; col < Config::COL_NUM; ++col)
			{
				auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

				Tile* tile = new Tile();//Create empty tile
				tile->getTransform()->position = glm::vec2(col * tileSize, row * tileSize);
				tile->setGridPosition(col, row);
				addChild(tile,1);
				tile->addLabels();
				tile->setTileCost(tile->getGridPosition().x);
				tile->setTileStatus(tile->getGridPosition().y);
				tile->setEnabled(false);
				if (tile->getGridPosition() == glm::vec2(8, 3))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(0, 3))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(11, 3))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(19, 3))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(12, 8))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(12, 12))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(16, 12))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(16, 8))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(7, 8))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(7, 12))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(3, 12))
					m_MovementNodes.push_back(tile);
				if (tile->getGridPosition() == glm::vec2(3, 8))
					m_MovementNodes.push_back(tile);
				
				m_pGrid.push_back(tile);
			}
		}
	//Create references for each tile to its neighbours
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			Tile* tile = m_getTile(col, row);

			//Topmost Row
			if (row == 0)
			{
				tile->setNeighbourTile(TOP_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(TOP_TILE, m_getTile(col, row - 1));
			}

			//Rightmost column
			if (col == Config::COL_NUM - 1)
			{
				tile->setNeighbourTile(RIGHT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(RIGHT_TILE, m_getTile(col + 1, row));

			}

			//BottomMost Row
			if (row == Config::ROW_NUM - 1)
			{
				tile->setNeighbourTile(BOTTOM_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(BOTTOM_TILE, m_getTile(col, row + 1));

			}

			//Leftmost Column
			if (col == 0)
			{
				tile->setNeighbourTile(LEFT_TILE, nullptr);
			}
			else
			{
				tile->setNeighbourTile(LEFT_TILE, m_getTile(col - 1, row));
			}
		}

	}

	//m_setGridEnabled(true);//TODO delete to set Tile info to false

	for (auto t : m_MovementNodes)
	{
		t->m_tileColor = glm::vec4(0, 0, 1, 1);
		t->setTileCost(88.8f);
		t->setTileStatus(88.8f);
	}
	
	std::cout << m_pGrid.size() << std::endl;
}

void PlayScene::m_buildGridSight()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);
			path_node->setEnabled(false);
			addChild(path_node, 5); // 04 86
			//Exceptions and creation
			{
				if ((col == 0 && row == 4) || (col == 1 && row == 4) || (col == 2 && row == 4) || (col == 3 && row == 4) || (col == 4 && row == 4) ||
					(col == 5 && row == 4) || (col == 6 && row == 4) || (col == 7 && row == 4) || (col == 8 && row == 4) ||
					(col == 0 && row == 5) || (col == 1 && row == 5) || (col == 2 && row == 5) || (col == 3 && row == 5) ||
					(col == 4 && row == 5) || (col == 5 && row == 5) || (col == 6 && row == 5) || (col == 7 && row == 5) || (col == 8 && row == 5) ||
					(col == 0 && row == 6) || (col == 1 && row == 6) || (col == 2 && row == 6) || (col == 3 && row == 6) ||
					(col == 4 && row == 6) || (col == 5 && row == 6) || (col == 6 && row == 6) || (col == 7 && row == 6) || (col == 8 && row == 6) ||
					(col == 11 && row == 4) || (col == 12 && row == 4) || (col == 13 && row == 4) || (col == 14 && row == 4) || (col == 15 && row == 4) ||
					(col == 16 && row == 4) || (col == 17 && row == 4) || (col == 18 && row == 4) || (col == 19 && row == 4) ||
					(col == 11 && row == 5) || (col == 12 && row == 5) || (col == 13 && row == 5) || (col == 14 && row == 5) ||
					(col == 15 && row == 5) || (col == 16 && row == 5) || (col == 17 && row == 5) || (col == 18 && row == 5) || (col == 19 && row == 5) ||
					(col == 11 && row == 6) || (col == 12 && row == 6) || (col == 13 && row == 6) || (col == 14 && row == 6) ||
					(col == 15 && row == 6) || (col == 16 && row == 6) || (col == 17 && row == 6) || (col == 18 && row == 6) || (col == 19 && row == 6))
				{

				}// 49 6 11
				else if ((col == 4 && row == 9) || (col == 4 && row == 10) || (col == 4 && row == 11) || (col == 5 && row == 9) || (col == 5 && row == 10) ||
					(col == 5 && row == 11) || (col == 6 && row == 9) || (col == 6 && row == 10) || (col == 6 && row == 11))
				{
					m_pLeftTreeNodes.push_back(path_node);
				}
				else if ((col == 13 && row == 9) || (col == 13 && row == 10) || (col == 13 && row == 11) || (col == 14 && row == 9) || (col == 14 && row == 10) ||
					(col == 14 && row == 11) || (col == 15 && row == 9) || (col == 15 && row == 10) || (col == 15 && row == 11))
				{
					m_pRightTreeNodes.push_back(path_node);
				}
				else //Create grid with remaining nodes
					m_pSGrid.push_back(path_node);
			}
		}
	}
}

void PlayScene::m_CheckPathNodeLOS()
{
	for (auto path_node : m_pSGrid)
	{
		auto targetDirection = m_pPlayer->getTransform()->position - path_node->getTransform()->position;
		auto normalizedDirection = Util::normalize(targetDirection);
		path_node->setOrientation(normalizedDirection);
		m_CheckShipLOS(path_node, m_pPlayer);
	}
}

void PlayScene::m_toggleGrid(bool state)
{
	for (auto path_node : m_pSGrid)
	{
		path_node->setEnabled(state);
	}
}

PathNode* PlayScene::m_findClosestPathNodeWithLOS(NavigationAgent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pSGrid)
	{
		if (path_node->hasLOS())
		{
			const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
			if (distance < min)
			{
				min = distance;
				closestPathNode = path_node;
			}
		}
	}

	return closestPathNode;
}

PathNode* PlayScene::m_findClosestPathNodeWithoutLOS(NavigationAgent* agent)
{
	auto min = INFINITY;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pSGrid)
	{
		if (!(path_node->hasLOS()))
		{
			const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
			if (distance < min)
			{
				min = distance;
				closestPathNode = path_node;
			}
		}
	}

	return closestPathNode;
}

void PlayScene::m_setGridEnabled(bool state) const
{
	for (auto tile : m_pGrid)
	{
		tile->setEnabled(state);
		tile->setLabelsEnabled(state);
	}
	if (state == false)
	{
		SDL_RenderClear(Renderer::Instance()->getRenderer());
	}
}

Tile* PlayScene::m_getTile(const int col, const int row) const
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

Tile* PlayScene::m_getTile(glm::vec2 grid_position) const
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;
	return m_pGrid[(row * Config::COL_NUM) + col];
}

void PlayScene::m_move()
{
	//State Machine stuff

	for (int i = 0; i < Enemies; i++)
	{
		// CloseCombat 0
		if (i == 0)
		{
			if (m_pEnemy[0]->active == true)
			{
				if (m_pPlayer->isEnabled())
				{
					auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

					if (m_pCloseCombatStateMachine->getCurrentState()->getAction()->getName() == "Patrol")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}
						if (m_pEnemy[i]->patrol == false)
						{
							m_pEnemy[i]->patrol = true;
						}

						m_pEnemy[i]->search = false;
						float dst2;
						if (m_pEnemy[i]->rightEnemy == true)
						{
							if (m_pEnemy[i]->p0 == false)
							{
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(12, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(12, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[0]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
						else if (m_pEnemy[i]->leftEnemy == true)
						{
							m_pEnemy[i]->search = false;
							float dst4;
							if (m_pEnemy[i]->p0 == false)
							{
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(7, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(7, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 8)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
					}
					else if (m_pCloseCombatStateMachine->getCurrentState()->getAction()->getName() == "MoveToLOS")
					{
						if (m_pEnemy[i]->search == false)
						{
							m_pEnemy[i]->move = true;
						}
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->setDestination(m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position);
						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position)
							< 2.0f)
						{
							m_pEnemy[i]->search = true;
							m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
							m_pEnemy[i]->move = false;
						}
					}
					else if (m_pCloseCombatStateMachine->getCurrentState()->getAction()->getName() == "MoveToPlayer")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
					}
					else if (m_pCloseCombatStateMachine->getCurrentState()->getAction()->getName() == "CloseCombatAttack")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->move = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						if (m_pEnemy[i]->AttackCd >= 3.0f)
						{
							m_CheckCloseCombat(m_pEnemy[i], m_pPlayer);
							if (m_pEnemy[i]->isInCloseCombatDistance())
							{
								if (CollisionManager::lineRectCheck(m_pEnemy[i]->getTransform()->position,
									m_pEnemy[i]->getTransform()->position + m_pEnemy[i]->getOrientation() * m_pEnemy[i]->getCloseCombatDistance(),
									m_pPlayer->getTransform()->position, m_pPlayer->getWidth(), m_pPlayer->getHeight()))
								{
									m_pEnemy[i]->AttackCd = 0;
									int h = 0;
									//Damage Player
									h = m_pPlayer->getCurrentHp();
									PlayerHp[h - 1]->setEnabled(false);
									SoundManager::Instance().playSound("dmg", 0, -1);
									m_pPlayer->setCurrentHp(m_pPlayer->getCurrentHp() - 1);
									if (m_pPlayer->getCurrentHp() == 0)
									{
										m_pPlayer->setEnabled(false);
										SoundManager::Instance().playSound("die", 0, -1);
									}
								}
							}
						}
					}
					else if (m_pCloseCombatStateMachine->getCurrentState()->getAction()->getName() == "Flee")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						m_pEnemy[i]->flee = true;
						if (m_pEnemy[i]->getTransform()->position.x<=0 || m_pEnemy[i]->getTransform()->position.x >= 800 || 
							m_pEnemy[i]->getTransform()->position.y <= 0 || m_pEnemy[i]->getTransform()->position.x >= 600)
						{
							RespawnCD = 0;
							fleed = true;
						}
					}
				}
			}
		}
		//Ranged 0
		else if (i == 1)
		{
			if (m_pEnemy[1]->active == true)
			{
				if (m_pPlayer->isEnabled())
				{
					auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

					if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "Patrol")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}
						if (m_pEnemy[i]->patrol == false)
						{
							m_pEnemy[i]->patrol = true;
						}

						m_pEnemy[i]->search = false;
						float dst2;
						if (m_pEnemy[i]->rightEnemy == true)
						{
							if (m_pEnemy[i]->p0 == false)
							{
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(12, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(12, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[0]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
						else if (m_pEnemy[i]->leftEnemy == true)
						{
							m_pEnemy[i]->search = false;
							float dst4;
							if (m_pEnemy[i]->p0 == false)
							{
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(7, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(7, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 8)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
					}
					else if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "MoveToPlayer")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
					}
					else if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "MoveToLOS")
					{
						if (m_pEnemy[i]->search == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->setDestination(m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position);
						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position)
							< 2.0f)
						{
							m_pEnemy[i]->search = true;
							m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
							m_pEnemy[i]->move = false;
						}
					}
					else if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "RangedAttack")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->move = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						if (m_pPlayer->isEnabled() == true)
						{
							if ((m_pEnemy[i]->getTransform()->position.x > 0.0f) && (m_pEnemy[i]->getTransform()->position.x < 800.0f))
							{
								if ((m_pEnemy[i]->getTransform()->position.y > 0.0f) && (m_pEnemy[i]->getTransform()->position.y < 600.0f))
								{
									if (m_pEnemy[i]->isEnabled() == true)
									{
										if (m_pEnemy[i]->AttackCd >= 3)
										{
											m_CheckRangedCombat(m_pEnemy[i], m_pPlayer);
											if (m_pEnemy[i]->isInRangedCombatDistance())
											{
												m_pEnemy[i]->AttackCd = 0;
												m_pEnemyBullet.push_back(new Bullet(m_pEnemy[i]->getRotation(), m_pEnemy[i]->getTransform()->position,
													"../Assets/textures/Tbullet2.png", "Tbullet2",true));
												addChild(m_pEnemyBullet[TotalEBullets]);
												TotalEBullets++;
											}
										}
									}
								}
							}
						}
					}
					else if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "Flee")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						m_pEnemy[i]->flee = true;
						if (m_pEnemy[i]->getTransform()->position.x <= 0 || m_pEnemy[i]->getTransform()->position.x >= 800 ||
							m_pEnemy[i]->getTransform()->position.y <= 0 || m_pEnemy[i]->getTransform()->position.x >= 600)
						{
							RespawnCD = 0;
							fleed = true;
						}
					}
					else if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "MoveBehindCover")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->setDestination(m_findClosestPathNodeWithoutLOS(m_pEnemy[i])->getTransform()->position);
						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_findClosestPathNodeWithoutLOS(m_pEnemy[i])->getTransform()->position)
							< 2.0f)
						{
							m_pEnemy[i]->search = false;
							m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
							m_pEnemy[i]->CoveringTime = 0;
							m_pEnemy[i]->isCovering = true;
							m_pEnemy[i]->isHit = false;
							m_pEnemy[i]->move = false;
						}
						else
						{
							m_pEnemy[i]->move = true;
						}
					}
					else if (m_pRangedStateMachine->getCurrentState()->getAction()->getName() == "WaitBehindCover")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						if (m_pEnemy[i]->wait == 0)
						{
							m_pEnemy[i]->wait = rand() % 4 + 2;

						}
						if (m_pEnemy[i]->CoveringTime >= m_pEnemy[i]->wait)
						{
							m_pEnemy[i]->isCovering = false;
							m_pEnemy[i]->wait = 0;
							m_pEnemy[i]->move = true;
						}
					}
				}
			}
		}
		// CloseCombat 2
		else if (i == 2)
		{
			if (m_pEnemy[2]->active == true)
			{
				if (m_pPlayer->isEnabled())
				{
					auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

					if (m_p2CloseCombatStateMachine->getCurrentState()->getAction()->getName() == "Patrol")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}
						if (m_pEnemy[i]->patrol == false)
						{
							m_pEnemy[i]->patrol = true;
						}

						m_pEnemy[i]->search = false;
						float dst2;
						if (m_pEnemy[i]->rightEnemy == true)
						{
							if (m_pEnemy[i]->p0 == false)
							{
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(12, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(12, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[0]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
						else if (m_pEnemy[i]->leftEnemy == true)
						{
							m_pEnemy[i]->search = false;
							float dst4;
							if (m_pEnemy[i]->p0 == false)
							{
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(7, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(7, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 8)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
					}
					else if (m_p2CloseCombatStateMachine->getCurrentState()->getAction()->getName() == "MoveToLOS")
					{
						if (m_pEnemy[i]->search == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->setDestination(m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position);
						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position)
							< 2.0f)
						{
							m_pEnemy[i]->search = true;
							m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
							m_pEnemy[i]->move = false;
						}
					}
					else if (m_p2CloseCombatStateMachine->getCurrentState()->getAction()->getName() == "MoveToPlayer")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
					}
					else if (m_p2CloseCombatStateMachine->getCurrentState()->getAction()->getName() == "CloseCombatAttack")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->move = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						if (m_pEnemy[i]->AttackCd >= 3.0f)
						{
							m_CheckCloseCombat(m_pEnemy[i], m_pPlayer);
							if (m_pEnemy[i]->isInCloseCombatDistance())
							{
								if (CollisionManager::lineRectCheck(m_pEnemy[i]->getTransform()->position,
									m_pEnemy[i]->getTransform()->position + m_pEnemy[i]->getOrientation() * m_pEnemy[i]->getCloseCombatDistance(),
									m_pPlayer->getTransform()->position, m_pPlayer->getWidth(), m_pPlayer->getHeight()))
								{
									m_pEnemy[i]->AttackCd = 0;
									int h = 0;
									//Damage Player
									h = m_pPlayer->getCurrentHp();
									PlayerHp[h - 1]->setEnabled(false);
									SoundManager::Instance().playSound("dmg", 0, -1);
									m_pPlayer->setCurrentHp(m_pPlayer->getCurrentHp() - 1);
									if (m_pPlayer->getCurrentHp() == 0)
									{
										m_pPlayer->setEnabled(false);
										SoundManager::Instance().playSound("die", 0, -1);
									}
								}
							}
						}
					}
					else if (m_p2CloseCombatStateMachine->getCurrentState()->getAction()->getName() == "Flee")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						m_pEnemy[i]->flee = true;
						if (m_pEnemy[i]->getTransform()->position.x <= 0 || m_pEnemy[i]->getTransform()->position.x >= 800 ||
							m_pEnemy[i]->getTransform()->position.y <= 0 || m_pEnemy[i]->getTransform()->position.x >= 600)
						{
							RespawnCD = 0;
							fleed = true;
						}
					}
				}
			}
		}
		//Ranged 2
		else if (i == 3)
		{
			if (m_pEnemy[3]->active == true)
			{
				if (m_pPlayer->isEnabled())
				{
					auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

					if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "Patrol")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}
						if (m_pEnemy[i]->patrol == false)
						{
							m_pEnemy[i]->patrol = true;
						}

						m_pEnemy[i]->search = false;
						float dst2;
						if (m_pEnemy[i]->rightEnemy == true)
						{
							if (m_pEnemy[i]->p0 == false)
							{
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(12, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(12, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(12, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[0]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 12)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
								dst2 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(16, 8)->getTransform()->position + offset);
								if (dst2 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
						else if (m_pEnemy[i]->leftEnemy == true)
						{
							m_pEnemy[i]->search = false;
							float dst4;
							if (m_pEnemy[i]->p0 == false)
							{
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 8)->getTransform()->position + offset);
								m_pEnemy[i]->setDestination(m_getTile(7, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = true;
								}
							}
							if (m_pEnemy[i]->p0 == true)
							{
								m_pEnemy[i]->setDestination(m_getTile(7, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(7, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p1 = true;
								}
							}
							if ((m_pEnemy[i]->p0 == true) && (m_pEnemy[i]->p1 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 12)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 12)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p2 = true;
								}
							}
							if ((m_pEnemy[i]->p1 == true) && (m_pEnemy[i]->p2 == true))
							{
								m_pEnemy[i]->setDestination(m_getTile(3, 8)->getTransform()->position + offset);
								dst4 = Util::distance(m_pEnemy[i]->getTransform()->position, m_getTile(3, 8)->getTransform()->position + offset);
								if (dst4 < 5.0f)
								{
									m_pEnemy[i]->p0 = false;
									m_pEnemy[i]->p1 = false;
									m_pEnemy[i]->p2 = false;
								}
							}
						}
					}
					else if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "MoveToPlayer")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
					}
					else if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "MoveToLOS")
					{
						if (m_pEnemy[i]->search == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->setDestination(m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position);
						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_findClosestPathNodeWithLOS(m_pEnemy[i])->getTransform()->position)
							< 2.0f)
						{
							m_pEnemy[i]->search = true;
							m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
							m_pEnemy[i]->move = false;
						}
					}
					else if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "RangedAttack")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->move = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						if (m_pPlayer->isEnabled() == true)
						{
							if ((m_pEnemy[i]->getTransform()->position.x > 0.0f) && (m_pEnemy[i]->getTransform()->position.x < 800.0f))
							{
								if ((m_pEnemy[i]->getTransform()->position.y > 0.0f) && (m_pEnemy[i]->getTransform()->position.y < 600.0f))
								{
									if (m_pEnemy[i]->isEnabled() == true)
									{
										if (m_pEnemy[i]->AttackCd >= 3)
										{
											m_CheckRangedCombat(m_pEnemy[i], m_pPlayer);
											if (m_pEnemy[i]->isInRangedCombatDistance())
											{
												m_pEnemy[i]->AttackCd = 0;
												m_pEnemyBullet.push_back(new Bullet(m_pEnemy[i]->getRotation(), m_pEnemy[i]->getTransform()->position,
													"../Assets/textures/Tbullet2.png", "Tbullet2",true));
												addChild(m_pEnemyBullet[TotalEBullets]);
												TotalEBullets++;
											}
										}
									}
								}
							}
						}
					}
					else if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "Flee")
					{
						if (m_pEnemy[i]->move == false)
						{
							m_pEnemy[i]->move = true;
						}

						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
						m_pEnemy[i]->flee = true;
						if (m_pEnemy[i]->getTransform()->position.x <= 0 || m_pEnemy[i]->getTransform()->position.x >= 800 ||
							m_pEnemy[i]->getTransform()->position.y <= 0 || m_pEnemy[i]->getTransform()->position.x >= 600)
						{
							RespawnCD = 0;
							fleed = true;
						}
					}
					else if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "MoveBehindCover")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->setDestination(m_findClosestPathNodeWithoutLOS(m_pEnemy[i])->getTransform()->position);
						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_findClosestPathNodeWithoutLOS(m_pEnemy[i])->getTransform()->position)
							< 2.0f)
						{
							m_pEnemy[i]->search = false;
							m_pEnemy[i]->setDestination(m_pPlayer->getTransform()->position);
							m_pEnemy[i]->CoveringTime = 0;
							m_pEnemy[i]->isCovering = true;
							m_pEnemy[i]->isHit = false;
							m_pEnemy[i]->move = false;
						}
						else
						{
							m_pEnemy[i]->move = true;
						}
					}
					else if (m_p2RangedStateMachine->getCurrentState()->getAction()->getName() == "WaitBehindCover")
					{
						m_pEnemy[i]->patrol = false;
						m_pEnemy[i]->search = false;
						if (m_pEnemy[i]->wait == 0)
						{
							m_pEnemy[i]->wait = rand() % 4 + 2;

						}
						if (m_pEnemy[i]->CoveringTime >= m_pEnemy[i]->wait)
						{
							m_pEnemy[i]->isCovering = false;
							m_pEnemy[i]->wait = 0;
							m_pEnemy[i]->move = true;
						}
					}
				}
			}
		}
	}
}

void PlayScene::m_CheckShipLOS(NavigationAgent* object,DisplayObject* To)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(object->getTransform()->position, To->getTransform()->position);
	if (ShipToTargetDistance <= object->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto obj : m_pMap)
		{
			if (obj->isEnabled())
			{
				//Check if object is farther than the target
				auto ShipToObjectDistance = Util::distance(object->getTransform()->position, obj->getTransform()->position);
				if (ShipToObjectDistance <= ShipToTargetDistance)
				{
					if ((obj->getType() != object->getType()) && (object->getType() != To->getType()))
					{
						contactList.push_back(obj);
					}
				}
			}
		}
		contactList.push_back(To); //add the target at the end of the list
		auto hasLOS = CollisionManager::LOSCheck(object->getTransform()->position,
			object->getTransform()->position + object->getOrientation() * object->getLOSDistance(),
			contactList, To);
		object->setHasLOS(hasLOS);
	}
}

void PlayScene::m_CheckCloseCombat(NavigationAgent* object,DisplayObject* To)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(object->getTransform()->position, To->getTransform()->position);
	if (ShipToTargetDistance <= object->getCloseCombatDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto obj : m_pMap)
		{
			if (obj->isEnabled())
			{
				//Check if object is farther than the target
				auto ShipToObjectDistance = Util::distance(object->getTransform()->position, obj->getTransform()->position);
				if (ShipToObjectDistance <= ShipToTargetDistance)
				{
					if ((obj->getType() != object->getType()) && (object->getType() != To->getType()))
					{
						contactList.push_back(obj);
					}
				}
			}
		}
		contactList.push_back(To); //add the target at the end of the list
		auto hasLOS = CollisionManager::LOSCheck(object->getTransform()->position,
			object->getTransform()->position + object->getOrientation() * object->getCloseCombatDistance(),
			contactList, To);
		object->setIsInCloseCombatDistance(hasLOS);
	}
}

void PlayScene::m_CheckRangedCombat(NavigationAgent* object, DisplayObject* To)
{

	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(object->getTransform()->position, To->getTransform()->position);
	if (ShipToTargetDistance <= object->getRangedCombatDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto obj : m_pMap)
		{
			if (obj->isEnabled())
			{
				//Check if object is farther than the target
				auto ShipToObjectDistance = Util::distance(object->getTransform()->position, obj->getTransform()->position);
				if (ShipToObjectDistance <= ShipToTargetDistance)
				{
					if ((obj->getType() != object->getType()) && (object->getType() != To->getType()))
					{
						contactList.push_back(obj);
					}
				}
			}
		}
		contactList.push_back(To); //add the target at the end of the list
		auto hasLOS = CollisionManager::LOSCheck(object->getTransform()->position,
			object->getTransform()->position + object->getOrientation() * object->getRangedCombatDistance(),
			contactList, To);
		object->setIsInRangedCombatDistance(hasLOS);
	}
}

void PlayScene::m_buildCloseCombatStateMachine()
{
	// Define States
	CloseCombatPatrolState = new State();
	State* CloseCombatMoveToPlayerState = new State();
	State* CloseCombatMoveToLOSState = new State();
	State* CloseCombatAttackState = new State();
	State* CloseCombatFleeState = new State();

	// Define Conditions
	m_pCloseCombatHasLOSCondition = new Condition();
	m_pCloseCombatLostLOSCondition = new Condition();
	m_pCloseCombatIsWithinDetectionRadiusCondition = new Condition();
	m_pCloseCombatIsNotWithinDetectionRadiusCondition = new Condition();
	m_pCloseCombatIsWithinCombatRangeCondition = new Condition();
	//
	m_pCLoseCombatLifeIsLow = new Condition();
	m_pCloseCombatNotWithinCombatRangeCondition = new Condition();

	// Define Transitions
	Transition* CloseCombatmoveToPlayerTransition = new Transition(m_pCloseCombatHasLOSCondition, CloseCombatMoveToPlayerState);
	Transition* CloseCombatmoveToLOSTransition = new Transition(m_pCloseCombatIsWithinDetectionRadiusCondition, CloseCombatMoveToLOSState);
	Transition* CloseCombatattackTransition = new Transition(m_pCloseCombatIsWithinCombatRangeCondition, CloseCombatAttackState);
	Transition* CloseCombatLOSToPatrolTransition = new Transition(m_pCloseCombatIsNotWithinDetectionRadiusCondition, CloseCombatPatrolState);
	Transition* CloseCombatmoveToPlayerToLOSTransition = new Transition(m_pCloseCombatLostLOSCondition, CloseCombatMoveToLOSState);
	//
	Transition* CloseCombatenemyFleesTransition = new Transition(m_pCLoseCombatLifeIsLow, CloseCombatFleeState);
	Transition* CloseCombatattackToMoveToPlayerTansition = new Transition(m_pCloseCombatNotWithinCombatRangeCondition, CloseCombatMoveToPlayerState);
	// Define Actions
	Patrol* CloseCombatPatrolAction = new Patrol();
	MoveToLOS* CloseCombatMoveToLOSAction = new MoveToLOS();
	MoveToPlayer* CloseCombatMoveToPlayerAction = new MoveToPlayer();
	CloseCombatAttack* CloseCombatAttackAction = new CloseCombatAttack();
	Flee* CloseCombatFleeAction = new Flee();

	// Setup Patrol State
	CloseCombatPatrolState->addTransition(CloseCombatmoveToPlayerTransition);
	CloseCombatPatrolState->addTransition(CloseCombatmoveToLOSTransition);
	CloseCombatPatrolState->addTransition(CloseCombatenemyFleesTransition);
	CloseCombatPatrolState->setAction(CloseCombatPatrolAction);

	// Setup MoveToPlayer State
	CloseCombatMoveToPlayerState->addTransition(CloseCombatattackTransition);
	CloseCombatMoveToPlayerState->addTransition(CloseCombatmoveToPlayerToLOSTransition);
	CloseCombatMoveToPlayerState->addTransition(CloseCombatenemyFleesTransition);
	CloseCombatMoveToPlayerState->setAction(CloseCombatMoveToPlayerAction);

	// Setup MoveToLOS State
	CloseCombatMoveToLOSState->addTransition(CloseCombatmoveToPlayerTransition);
	CloseCombatMoveToLOSState->addTransition(CloseCombatLOSToPatrolTransition);
	CloseCombatMoveToLOSState->addTransition(CloseCombatenemyFleesTransition);
	CloseCombatMoveToLOSState->setAction(CloseCombatMoveToLOSAction);

	// Setup CloseCombatAttack State
	CloseCombatAttackState->addTransition(CloseCombatattackToMoveToPlayerTansition); // Missing Condition
	CloseCombatAttackState->addTransition(CloseCombatmoveToPlayerToLOSTransition); // Missing Condition
	CloseCombatAttackState->addTransition(CloseCombatenemyFleesTransition);
	CloseCombatAttackState->setAction(CloseCombatAttackAction);

	// Flee State
	CloseCombatFleeState->setAction(CloseCombatFleeAction);

	m_pCloseCombatStateMachine = new StateMachine();
	m_pCloseCombatStateMachine->setCurrentState(CloseCombatPatrolState);
}

void PlayScene::m_buildCloseCombatStateMachine2()
{
	// Define States
	CloseCombat2PatrolState = new State();
	State* CloseCombat2MoveToPlayerState = new State();
	State* CloseCombat2MoveToLOSState = new State();
	State* CloseCombat2AttackState = new State();
	State* CloseCombat2FleeState = new State();

	// Define Conditions
	m_p2CloseCombatHasLOSCondition = new Condition();
	m_p2CloseCombatLostLOSCondition = new Condition();
	m_p2CloseCombatIsWithinDetectionRadiusCondition = new Condition();
	m_p2CloseCombatIsNotWithinDetectionRadiusCondition = new Condition();
	m_p2CloseCombatIsWithinCombatRangeCondition = new Condition();
	//
	m_p2CLoseCombatLifeIsLow = new Condition();
	m_p2CloseCombatNotWithinCombatRangeCondition = new Condition();

	// Define Transitions
	Transition* CloseCombat2moveToPlayerTransition = new Transition(m_p2CloseCombatHasLOSCondition, CloseCombat2MoveToPlayerState);
	Transition* CloseCombat2moveToLOSTransition = new Transition(m_p2CloseCombatIsWithinDetectionRadiusCondition, CloseCombat2MoveToLOSState);
	Transition* CloseCombat2attackTransition = new Transition(m_p2CloseCombatIsWithinCombatRangeCondition, CloseCombat2AttackState);
	Transition* CloseCombat2LOSToPatrolTransition = new Transition(m_p2CloseCombatIsNotWithinDetectionRadiusCondition, CloseCombat2PatrolState);
	Transition* CloseCombat2moveToPlayerToLOSTransition = new Transition(m_p2CloseCombatLostLOSCondition, CloseCombat2MoveToLOSState);
	//					   
	Transition* CloseCombat2enemyFleesTransition = new Transition(m_p2CLoseCombatLifeIsLow, CloseCombat2FleeState);
	Transition* CloseCombat2attackToMoveToPlayerTansition = new Transition(m_p2CloseCombatNotWithinCombatRangeCondition, CloseCombat2MoveToPlayerState);
	// Define Actions
	Patrol* CloseCombat2PatrolAction = new Patrol();
	MoveToLOS* CloseCombat2MoveToLOSAction = new MoveToLOS();
	MoveToPlayer* CloseCombat2MoveToPlayerAction = new MoveToPlayer();
	CloseCombatAttack* CloseCombat2AttackAction = new CloseCombatAttack();
	Flee* CloseCombat2FleeAction = new Flee();

	// Setup Patrol State
	CloseCombat2PatrolState->addTransition(CloseCombat2moveToPlayerTransition);
	CloseCombat2PatrolState->addTransition(CloseCombat2moveToLOSTransition);
	CloseCombat2PatrolState->addTransition(CloseCombat2enemyFleesTransition);
	CloseCombat2PatrolState->setAction(CloseCombat2PatrolAction);

	// Setup MoveToPlayer State
	CloseCombat2MoveToPlayerState->addTransition(CloseCombat2attackTransition);
	CloseCombat2MoveToPlayerState->addTransition(CloseCombat2moveToPlayerToLOSTransition);
	CloseCombat2MoveToPlayerState->addTransition(CloseCombat2enemyFleesTransition);
	CloseCombat2MoveToPlayerState->setAction(CloseCombat2MoveToPlayerAction);

	// Setup MoveToLOS State
	CloseCombat2MoveToLOSState->addTransition(CloseCombat2moveToPlayerTransition);
	CloseCombat2MoveToLOSState->addTransition(CloseCombat2LOSToPatrolTransition);
	CloseCombat2MoveToLOSState->addTransition(CloseCombat2enemyFleesTransition);
	CloseCombat2MoveToLOSState->setAction(CloseCombat2MoveToLOSAction);

	// Setup CloseCombatAttack State
	CloseCombat2AttackState->addTransition(CloseCombat2attackToMoveToPlayerTansition); // Missing Condition
	CloseCombat2AttackState->addTransition(CloseCombat2moveToPlayerToLOSTransition); // Missing Condition
	CloseCombat2AttackState->addTransition(CloseCombat2enemyFleesTransition);
	CloseCombat2AttackState->setAction(CloseCombat2AttackAction);

	// Flee State
	CloseCombat2FleeState->setAction(CloseCombat2FleeAction);

	m_p2CloseCombatStateMachine = new StateMachine();
	m_p2CloseCombatStateMachine->setCurrentState(CloseCombat2PatrolState);
}

void PlayScene::m_buildRangedStateMachine()
{
	// Define States
	RangedPatrolState = new State();
	State* RangedMoveToPlayerState = new State();
	State* RangedMoveToLOSState = new State();
	State* RangedAttackState = new State();
	State* RangedFleeState = new State();
	State* RangedMoveBehindCoverState = new State();
	State* RangedWaitBehindCoverState = new State();

	// Define Conditions
	m_pRangedHasLOSCondition = new Condition();
	m_pRangedLostLOSCondition = new Condition();
	m_pRangedIsWithinDetectionRadiusCondition = new Condition();
	m_pRangedIsNotWithinDetectionRadiusCondition = new Condition();
	m_pRangedIsWithinCombatRangeCondition = new Condition();
	//
	m_pRangedLifeIsLow = new Condition();
	m_pRangedNotWithinCombatRangeCondition = new Condition();
	m_pRangedIsHit = new Condition();
	m_pRangedCoverWait = new Condition();
	m_pRangedCoverOut = new Condition();

	// Define Transitions
	Transition* RangedMoveToPlayerTransition = new Transition(m_pRangedHasLOSCondition, RangedMoveToPlayerState);
	Transition* RangedMoveToLOSTransition = new Transition(m_pRangedIsWithinDetectionRadiusCondition, RangedMoveToLOSState);
	Transition* RangedAttackTransition = new Transition(m_pRangedIsWithinCombatRangeCondition, RangedAttackState);
	Transition* RangedMLOSToPatrolTransition = new Transition(m_pRangedIsNotWithinDetectionRadiusCondition, RangedPatrolState);
	Transition* RangedMoveToPlayerToLOSTransition = new Transition(m_pRangedLostLOSCondition, RangedMoveToLOSState);
	//
	Transition* RangedEnemyFleesTransition = new Transition(m_pRangedLifeIsLow, RangedFleeState);
	Transition* RangedAttackToMoveToPlayerTansition = new Transition(m_pRangedNotWithinCombatRangeCondition, RangedMoveToPlayerState);
	Transition* RangedMoveBehindCoverTransition = new Transition(m_pRangedIsHit, RangedMoveBehindCoverState);
	Transition* RangedCoverWaitTransition = new Transition(m_pRangedCoverWait, RangedWaitBehindCoverState);
	Transition* RangedOutOfCoverTransition = new Transition(m_pRangedCoverOut, RangedMoveToLOSState);

	// Define Actions
	Patrol* RangedPatrolAction = new Patrol();
	MoveToLOS* RangedMoveToLOSAction = new MoveToLOS();
	MoveToPlayer* RangedMoveToPlayerAction = new MoveToPlayer();
	RangedAttack* RangedAttackAction = new RangedAttack();
	MoveBehindCover* RangedMoveBehindCoverAction = new MoveBehindCover();
	WaitBehindCover* RangedWaitBehindCoverAction = new WaitBehindCover();
	Flee* RangedFleeAction = new Flee();

	// Setup Patrol State
	RangedPatrolState->addTransition(RangedMoveToPlayerTransition);
	RangedPatrolState->addTransition(RangedMoveToLOSTransition);
	RangedPatrolState->addTransition(RangedEnemyFleesTransition);
	RangedPatrolState->addTransition(RangedMoveBehindCoverTransition);
	RangedPatrolState->setAction(RangedPatrolAction);

	// Setup MoveToPlayer State
	RangedMoveToPlayerState->addTransition(RangedAttackTransition);
	RangedMoveToPlayerState->addTransition(RangedMoveToPlayerToLOSTransition);
	RangedMoveToPlayerState->addTransition(RangedEnemyFleesTransition);
	RangedMoveToPlayerState->addTransition(RangedMoveBehindCoverTransition);
	RangedMoveToPlayerState->setAction(RangedMoveToPlayerAction);

	// Setup MoveToLOS State
	RangedMoveToLOSState->addTransition(RangedMoveToPlayerTransition);
	RangedMoveToLOSState->addTransition(RangedMLOSToPatrolTransition);
	RangedMoveToLOSState->addTransition(RangedEnemyFleesTransition);
	RangedMoveToLOSState->addTransition(RangedMoveBehindCoverTransition);
	RangedMoveToLOSState->setAction(RangedMoveToLOSAction);

	// Setup CloseCombatAttack State
	RangedAttackState->addTransition(RangedAttackToMoveToPlayerTansition); 
	RangedAttackState->addTransition(RangedMoveToPlayerToLOSTransition); 
	RangedAttackState->addTransition(RangedEnemyFleesTransition);
	RangedAttackState->addTransition(RangedMoveBehindCoverTransition);
	RangedAttackState->setAction(RangedAttackAction);

	// Move Behind Cover State
	RangedMoveBehindCoverState->addTransition(RangedEnemyFleesTransition);
	RangedMoveBehindCoverState->addTransition(RangedCoverWaitTransition);
	RangedMoveBehindCoverState->setAction(RangedMoveBehindCoverAction);

	// Wait Behind Cover State
	RangedWaitBehindCoverState->addTransition(RangedEnemyFleesTransition);
	RangedWaitBehindCoverState->addTransition(RangedOutOfCoverTransition);
	RangedWaitBehindCoverState->setAction(RangedWaitBehindCoverAction);
	
	
	// Flee State
	RangedFleeState->setAction(RangedFleeAction);

	m_pRangedStateMachine = new StateMachine();
	m_pRangedStateMachine->setCurrentState(RangedPatrolState);
}

void PlayScene::m_buildRangedStateMachine2()
{
	// Define States
	Ranged2PatrolState = new State();
	State* Ranged2MoveToPlayerState = new State();
	State* Ranged2MoveToLOSState = new State();
	State* Ranged2AttackState = new State();
	State* Ranged2FleeState = new State();
	State* Ranged2MoveBehindCoverState = new State();
	State* Ranged2WaitBehindCoverState = new State();

	// Define Conditions
	m_p2RangedHasLOSCondition = new Condition();
	m_p2RangedLostLOSCondition = new Condition();
	m_p2RangedIsWithinDetectionRadiusCondition = new Condition();
	m_p2RangedIsNotWithinDetectionRadiusCondition = new Condition();
	m_p2RangedIsWithinCombatRangeCondition = new Condition();
	//
	m_p2RangedLifeIsLow = new Condition();
	m_p2RangedNotWithinCombatRangeCondition = new Condition();
	m_p2RangedIsHit = new Condition();
	m_p2RangedCoverWait = new Condition();
	m_p2RangedCoverOut = new Condition();

	// Define Transitions
	Transition* Ranged2MoveToPlayerTransition = new Transition(m_p2RangedHasLOSCondition, Ranged2MoveToPlayerState);
	Transition* Ranged2MoveToLOSTransition = new Transition(m_p2RangedIsWithinDetectionRadiusCondition, Ranged2MoveToLOSState);
	Transition* Ranged2AttackTransition = new Transition(m_p2RangedIsWithinCombatRangeCondition, Ranged2AttackState);
	Transition* Ranged2MLOSToPatrolTransition = new Transition(m_p2RangedIsNotWithinDetectionRadiusCondition, Ranged2PatrolState);
	Transition* Ranged2MoveToPlayerToLOSTransition = new Transition(m_p2RangedLostLOSCondition, Ranged2MoveToLOSState);
	//
	Transition* Ranged2EnemyFleesTransition = new Transition(m_p2RangedLifeIsLow, Ranged2FleeState);
	Transition* Ranged2AttackToMoveToPlayerTansition = new Transition(m_p2RangedNotWithinCombatRangeCondition, Ranged2MoveToPlayerState);
	Transition* Ranged2MoveBehindCoverTransition = new Transition(m_p2RangedIsHit, Ranged2MoveBehindCoverState);
	Transition* Ranged2CoverWaitTransition = new Transition(m_p2RangedCoverWait, Ranged2WaitBehindCoverState);
	Transition* Ranged2OutOfCoverTransition = new Transition(m_p2RangedCoverOut, Ranged2MoveToLOSState);

	// Define Actions
	Patrol* Ranged2PatrolAction = new Patrol();
	MoveToLOS* Ranged2MoveToLOSAction = new MoveToLOS();
	MoveToPlayer* Ranged2MoveToPlayerAction = new MoveToPlayer();
	RangedAttack* Ranged2AttackAction = new RangedAttack();
	MoveBehindCover* Ranged2MoveBehindCoverAction = new MoveBehindCover();
	WaitBehindCover* Ranged2WaitBehindCoverAction = new WaitBehindCover();
	Flee* Ranged2FleeAction = new Flee();

	// Setup Patrol State
	Ranged2PatrolState->addTransition(Ranged2MoveToPlayerTransition);
	Ranged2PatrolState->addTransition(Ranged2MoveToLOSTransition);
	Ranged2PatrolState->addTransition(Ranged2EnemyFleesTransition);
	Ranged2PatrolState->addTransition(Ranged2MoveBehindCoverTransition);
	Ranged2PatrolState->setAction(Ranged2PatrolAction);

	// Setup MoveToPlayer State
	Ranged2MoveToPlayerState->addTransition(Ranged2AttackTransition);
	Ranged2MoveToPlayerState->addTransition(Ranged2MoveToPlayerToLOSTransition);
	Ranged2MoveToPlayerState->addTransition(Ranged2EnemyFleesTransition);
	Ranged2MoveToPlayerState->addTransition(Ranged2MoveBehindCoverTransition);
	Ranged2MoveToPlayerState->setAction(Ranged2MoveToPlayerAction);

	// Setup MoveToLOS State
	Ranged2MoveToLOSState->addTransition(Ranged2MoveToPlayerTransition);
	Ranged2MoveToLOSState->addTransition(Ranged2MLOSToPatrolTransition);
	Ranged2MoveToLOSState->addTransition(Ranged2EnemyFleesTransition);
	Ranged2MoveToLOSState->addTransition(Ranged2MoveBehindCoverTransition);
	Ranged2MoveToLOSState->setAction(Ranged2MoveToLOSAction);

	// Setup CloseCombatAttack State
	Ranged2AttackState->addTransition(Ranged2AttackToMoveToPlayerTansition);
	Ranged2AttackState->addTransition(Ranged2MoveToPlayerToLOSTransition);
	Ranged2AttackState->addTransition(Ranged2EnemyFleesTransition);
	Ranged2AttackState->addTransition(Ranged2MoveBehindCoverTransition);
	Ranged2AttackState->setAction(Ranged2AttackAction);

	// Move Behind Cover State
	Ranged2MoveBehindCoverState->addTransition(Ranged2EnemyFleesTransition);
	Ranged2MoveBehindCoverState->addTransition(Ranged2CoverWaitTransition);
	Ranged2MoveBehindCoverState->setAction(Ranged2MoveBehindCoverAction);

	// Wait Behind Cover State
	Ranged2WaitBehindCoverState->addTransition(Ranged2EnemyFleesTransition);
	Ranged2WaitBehindCoverState->addTransition(Ranged2OutOfCoverTransition);
	Ranged2WaitBehindCoverState->setAction(Ranged2WaitBehindCoverAction);


	// Flee State
	Ranged2FleeState->setAction(Ranged2FleeAction);

	m_p2RangedStateMachine = new StateMachine();
	m_p2RangedStateMachine->setCurrentState(Ranged2PatrolState);
}
