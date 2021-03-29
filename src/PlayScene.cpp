#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include"DecisionTree.h"


// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();

	SoundManager::Instance().load("../Assets/audio/Bgm2.mp3", "Bgm2", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/Attack.mp3", "at", SOUND_SFX);
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
		m_CheckShipLOS(m_pEnemy[i]);
		m_CheckShipLOS(m_pEnemyDebug[i]);
		m_CheckShipCloseCombatPlayer(m_pEnemy[i]);
	}

	//Enemy trees TODO
	for (int i = 0; i < Enemies; i++)
	{
		decisionTree[i]->MakeDecision();
	}


	//Enemy movevents
	m_move();

	//Timer for Cooldowns
	GameTimer += 1 * deltaTime;
	ButtonCD += 1 * deltaTime;
	CheckCD += 1 * deltaTime;
	GunCD += 1 * deltaTime;

	//for (auto i = 0; i < 8; i++)
	//{
	//	m_pEnemy[i]->cd += 1 * deltaTime;
	//	m_pEnemy[i]->avocd += 1 * deltaTime;
	//}
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

	//Enemies hp bind
	//Enemy 0
	Enemy0->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x,m_pEnemy[0]->getTransform()->position.y - 40 };
	//Enemy 1
	Enemy1->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x,m_pEnemy[1]->getTransform()->position.y - 40 };
	//Enemy 2
	Enemy2[0]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x,m_pEnemy[2]->getTransform()->position.y - 40 };
	Enemy2[1]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x + 10,m_pEnemy[2]->getTransform()->position.y - 40 };
	//Enemy 3
	Enemy3[0]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x,m_pEnemy[3]->getTransform()->position.y - 40 };
	Enemy3[1]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x + 10,m_pEnemy[3]->getTransform()->position.y - 40 };
	//Enemy 4
	Enemy4[0]->getTransform()->position = { m_pEnemy[4]->getTransform()->position.x,m_pEnemy[4]->getTransform()->position.y - 40 };
	Enemy4[1]->getTransform()->position = { m_pEnemy[4]->getTransform()->position.x + 10,m_pEnemy[4]->getTransform()->position.y - 40 };
	//Enemy 5
	Enemy5[0]->getTransform()->position = { m_pEnemy[5]->getTransform()->position.x,m_pEnemy[5]->getTransform()->position.y - 40 };
	Enemy5[1]->getTransform()->position = { m_pEnemy[5]->getTransform()->position.x + 10,m_pEnemy[5]->getTransform()->position.y - 40 };
	
	//Player hp bind
	PlayerHp[0]->getTransform()->position = { m_pPlayer->getTransform()->position.x,m_pPlayer->getTransform()->position.y - 40 };
	PlayerHp[1]->getTransform()->position = { m_pPlayer->getTransform()->position.x + 10,m_pPlayer->getTransform()->position.y - 40 };
	PlayerHp[2]->getTransform()->position = { m_pPlayer->getTransform()->position.x - 10,m_pPlayer->getTransform()->position.y - 40 };

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
	for (int i = 0; i < 7;i++)
	{
		if (i==EnemiesDestroyed)
		{
			m_Inst[i]->setEnabled(true);
		}
		else
			m_Inst[i]->setEnabled(false);
	}
	
	//Collisions

	//Player and stage Collision

	//TODO fix
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
	}
	
	//Player and enemy Collision
	//for (int EnemyTanks = 0; EnemyTanks < 8; EnemyTanks++)
	//{
	//	if (m_pEnemy[EnemyTanks]->isEnabled() == true)
	//	{
	//		if (m_pPlayer->isEnabled() == true)
	//		{
	//			if (CollisionManager::CircleAABBTanks(m_pPlayer, m_pEnemy[EnemyTanks]))
	//			{
	//				m_pPlayer->setEnabled(false);
	//				m_pPlayerTurret->setEnabled(false);
	//				m_pEnemy[EnemyTanks]->setEnabled(false);
	//				m_pETurret[EnemyTanks]->setEnabled(false);
	//				SoundManager::Instance().playSound("Expl", 0, -1);
	//			}
	//		}
	//	}
	//}
	//TODO Tank 2or3 bug
	//Player bullet and enemy tank collision
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
							Enemy0->setEnabled(false);
							m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
							if (m_pEnemy[y]->getCurrentHp() == 0)
							{
								m_pEnemy[y]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, -1);
								if (m_pEnemyDebug[y]->isEnabled())
									m_pEnemyDebug[y]->setEnabled(false);
							}
						}
						//Damage Enemy1
						else if (y == 1)
						{
							Enemy1->setEnabled(false);
							m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
							if (m_pEnemy[y]->getCurrentHp() == 0)
							{
								m_pEnemy[y]->setEnabled(false);
								SoundManager::Instance().playSound("dmg", 0, -1);
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
							m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
							if (m_pEnemy[y]->getCurrentHp() == 0)
							{
								m_pEnemy[y]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, -1);
								if (m_pEnemyDebug[y]->isEnabled())
									m_pEnemyDebug[y]->setEnabled(false);
							}
						}
						//Damage Enemy3
						else if (i == 3)
						{
							h = m_pEnemy[y]->getCurrentHp();
							Enemy3[h - 1]->setEnabled(false);
							m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
							if (m_pEnemy[y]->getCurrentHp() == 0)
							{
								m_pEnemy[y]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, -1);
								if (m_pEnemyDebug[y]->isEnabled())
									m_pEnemyDebug[y]->setEnabled(false);
							}
						}
						//Damage Enemy4
						else if (y == 4)
						{
							h = m_pEnemy[y]->getCurrentHp();
							Enemy4[h - 1]->setEnabled(false);
							m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
							if (m_pEnemy[y]->getCurrentHp() == 0)
							{
								m_pEnemy[y]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, -1);
								if (m_pEnemyDebug[y]->isEnabled())
									m_pEnemyDebug[y]->setEnabled(false);
							}
						}
						//Damage Enemy5
						else if (y == 5)
						{
							h = m_pEnemy[y]->getCurrentHp();
							Enemy5[h - 1]->setEnabled(false);
							m_pEnemy[y]->setCurrentHp(m_pEnemy[y]->getCurrentHp() - 1);
							if (m_pEnemy[y]->getCurrentHp() == 0)
							{
								m_pEnemy[y]->setEnabled(false);
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
	
	//Enemy Bullet and player Tank Collision
	//if (m_pPlayer->isEnabled() == true)
	//{
	//	for (int i = 0; i < m_pEnemyBullet.size(); i++)
	//	{
	//		if (m_pEnemyBullet[i]->isEnabled())
	//		{
	//			if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_pPlayer))
	//			{
	//				m_pEnemyBullet[i]->setEnabled(false);
	//				m_pPlayer->setEnabled(false);
	//				m_pPlayerTurret->setEnabled(false);
	//				SoundManager::Instance().playSound("Expl", 0, -1);
	//			}
	//		}
	//	}
	//}

	//Enemy Bullet and stage Collision

	//for (int i = 0; i < m_pEnemyBullet.size(); i++)
	//{
	//	if (m_pEnemyBullet[i]->isEnabled() == true)
	//	{
	//		for (int u = 0; u < 12; u++)
	//		{
	//			if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_field[u]))
	//			{
	//				m_pEnemyBullet[i]->setEnabled(false);

	//				SoundManager::Instance().playSound("Expl", 0, -1);
	//			}
	//		}
	//	}
	//}

	//Avoidance TODO
	//for (int EnemyTanks = 0; EnemyTanks < 8; EnemyTanks++)
	//{
	//	if(m_pEnemy[EnemyTanks]->seek==true)
	//	{
	//		for (int y = 0; y < 12; y++)
	//		{
	//			//Left whishker
	//			if (CollisionManager::lineRectCheck(m_pEnemy[EnemyTanks]->m_LWhishker.Start(),
	//				m_pEnemy[EnemyTanks]->m_LWhishker.End(), m_field[y]->getTransform()->position-
	//				glm::vec2(m_field[y]->getWidth()/2, m_field[y]->getHeight()/2),
	//				m_field[y]->getWidth(), m_field[y]->getHeight()))
	//			{
	//				m_pEnemy[EnemyTanks]->ColObsL = y;
	//				m_pEnemy[EnemyTanks]->tRight = true;
	//				m_pEnemy[EnemyTanks]->avoidance = true;
	//			}
	//			if(m_pEnemy[EnemyTanks]->ColObsL!=100)
	//			{
	//				if (!(CollisionManager::lineRectCheck(m_pEnemy[EnemyTanks]->m_LWhishker.Start(),
	//					m_pEnemy[EnemyTanks]->m_LWhishker.End(), m_field[m_pEnemy[EnemyTanks]->ColObsL]->getTransform()->position -
	//					glm::vec2(m_field[m_pEnemy[EnemyTanks]->ColObsL]->getWidth() / 2, m_field[m_pEnemy[EnemyTanks]->ColObsL]->getHeight() / 2),
	//					m_field[m_pEnemy[EnemyTanks]->ColObsL]->getWidth(), m_field[m_pEnemy[EnemyTanks]->ColObsL]->getHeight())))
	//				{
	//					m_pEnemy[EnemyTanks]->tRight = false;
	//					m_pEnemy[EnemyTanks]->avocd = 0;
	//					if(m_pEnemy[EnemyTanks]->avocd>1)
	//					{
	//						m_pEnemy[EnemyTanks]->avoidance = false;
	//						m_pEnemy[EnemyTanks]->ColObsL = 100;
	//					}
	//				}
	//			}
	//			//Right Whishker
	//			if (CollisionManager::lineRectCheck(m_pEnemy[EnemyTanks]->m_RWhishker.Start(),
	//				m_pEnemy[EnemyTanks]->m_RWhishker.End(), m_field[y]->getTransform()->position -
	//				glm::vec2(m_field[y]->getWidth() / 2, m_field[y]->getHeight() / 2),
	//				m_field[y]->getWidth(), m_field[y]->getHeight()))
	//			{
	//				m_pEnemy[EnemyTanks]->ColObsR = y;
	//				m_pEnemy[EnemyTanks]->avoidance = true;
	//				m_pEnemy[EnemyTanks]->tLeft = true;
	//			}
	//			if (m_pEnemy[EnemyTanks]->ColObsR != 100)
	//			{
	//				if (!(CollisionManager::lineRectCheck(m_pEnemy[EnemyTanks]->m_RWhishker.Start(),
	//					m_pEnemy[EnemyTanks]->m_RWhishker.End(), m_field[m_pEnemy[EnemyTanks]->ColObsR]->getTransform()->position -
	//					glm::vec2(m_field[m_pEnemy[EnemyTanks]->ColObsR]->getWidth() / 2, m_field[m_pEnemy[EnemyTanks]->ColObsR]->getHeight() / 2),
	//					m_field[m_pEnemy[EnemyTanks]->ColObsR]->getWidth(), m_field[m_pEnemy[EnemyTanks]->ColObsR]->getHeight())))
	//				{
	//					m_pEnemy[EnemyTanks]->tLeft = false;
	//					m_pEnemy[EnemyTanks]->avocd = 0;
	//					if (m_pEnemy[EnemyTanks]->avocd > 1)
	//					{
	//						m_pEnemy[EnemyTanks]->avoidance = false;
	//						m_pEnemy[EnemyTanks]->ColObsL = 100;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
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
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K)) 
	{
		if(Debug==true)
		{
			if (ButtonCD > 1)
			{
				for (int i = 0; i < Enemies; i++)
				{

					int h;
					//Damage Enemy0
					if (i == 0)
					{
						Enemy0->setEnabled(false);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
					//Damage Enemy1
					else if (i == 1)
					{
						Enemy1->setEnabled(false);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
					//Damage Enemy2
					else if (i == 2)
					{
						h = m_pEnemy[i]->getCurrentHp();
						Enemy2[h - 1]->setEnabled(false);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
					//Damage Enemy3
					else if (i == 3)
					{
						h = m_pEnemy[i]->getCurrentHp();
						Enemy3[h - 1]->setEnabled(false);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
					//Damage Enemy4
					else if (i == 4)
					{
						h = m_pEnemy[i]->getCurrentHp();
						Enemy4[h - 1]->setEnabled(false);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
					//Damage Enemy5
					else if (i == 5)
					{
						h = m_pEnemy[i]->getCurrentHp();
						Enemy5[h - 1]->setEnabled(false);
						m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
						if (m_pEnemy[i]->getCurrentHp() == 0)
						{
							m_pEnemy[i]->setEnabled(false);
							EnemiesDestroyed++;
							if (m_pEnemyDebug[i]->isEnabled())
								m_pEnemyDebug[i]->setEnabled(false);
						}
					}
				}
				ButtonCD = 0;
			}
		}
	}

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

	//Player CloseCombat Attack
	if (EventManager::Instance().getMouseButton(0) && GunCD > 1)
	{
		if (m_pPlayer->isEnabled() == true)
		{
			SoundManager::Instance().playSound("at", 0, -1);
			for (int i = 0; i < Enemies; i++)
			{
				m_CheckShipCloseCombatPlayer(m_pEnemy[i]);
				if (m_pPlayer->isInCloseCombatDistance())
				{
					if (CollisionManager::lineRectCheck(m_pPlayer->getTransform()->position,
						m_pPlayer->getTransform()->position + m_pPlayer->getOrientation() * m_pPlayer->getCloseCombatDistance(),
						m_pEnemy[i]->getTransform()->position, m_pEnemy[i]->getWidth(), m_pEnemy[i]->getHeight()))
					{
						GunCD = 0;
						int h = 0;
						//Damage Enemy0
						if (i == 0)
						{
							Enemy0->setEnabled(false);
							m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
							SoundManager::Instance().playSound("dmg", 0, -1);
							if (m_pEnemy[i]->getCurrentHp() == 0)
							{
								m_pEnemy[i]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, 1);
								if (m_pEnemyDebug[i]->isEnabled())
									m_pEnemyDebug[i]->setEnabled(false);
							}
						}
						//Damage Enemy1
						else if (i == 1)
						{
							Enemy1->setEnabled(false);
							m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
							SoundManager::Instance().playSound("dmg", 0, -1);
							if (m_pEnemy[i]->getCurrentHp() == 0)
							{
								m_pEnemy[i]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, 1);
								if (m_pEnemyDebug[i]->isEnabled())
									m_pEnemyDebug[i]->setEnabled(false);
							}
						}
						//Damage Enemy2
						else if (i == 2)
						{
							h = m_pEnemy[i]->getCurrentHp();
							Enemy2[h-1]->setEnabled(false);
							m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp()-1);
							SoundManager::Instance().playSound("dmg", 0, -1);
							if (m_pEnemy[i]->getCurrentHp() == 0)
							{
								m_pEnemy[i]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, 1);
								if (m_pEnemyDebug[i]->isEnabled())
									m_pEnemyDebug[i]->setEnabled(false);
							}
						}
						//Damage Enemy3
						else if (i == 3)
						{
							h = m_pEnemy[i]->getCurrentHp();
							Enemy3[h-1]->setEnabled(false);
							m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
							SoundManager::Instance().playSound("dmg", 0, -1);
							if (m_pEnemy[i]->getCurrentHp() == 0)
							{
								m_pEnemy[i]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, 1);
								if (m_pEnemyDebug[i]->isEnabled())
									m_pEnemyDebug[i]->setEnabled(false);
							}
						}
						//Damage Enemy4
						else if (i == 4)
						{
							h = m_pEnemy[i]->getCurrentHp();
							Enemy4[h-1]->setEnabled(false);
							m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
							SoundManager::Instance().playSound("dmg", 0, -1);
							if (m_pEnemy[i]->getCurrentHp() == 0)
							{
								m_pEnemy[i]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, 1);
								if (m_pEnemyDebug[i]->isEnabled())
									m_pEnemyDebug[i]->setEnabled(false);
							}
						}
						//Damage Enemy5
						else if (i == 5)
						{
							h = m_pEnemy[i]->getCurrentHp();
							Enemy5[h-1]->setEnabled(false);
							m_pEnemy[i]->setCurrentHp(m_pEnemy[i]->getCurrentHp() - 1);
							SoundManager::Instance().playSound("dmg", 0, -1);
							if (m_pEnemy[i]->getCurrentHp() == 0)
							{
								m_pEnemy[i]->setEnabled(false);
								EnemiesDestroyed++;
								SoundManager::Instance().playSound("die", 0, 1);
								if (m_pEnemyDebug[i]->isEnabled())
									m_pEnemyDebug[i]->setEnabled(false);
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
			m_pBullet.push_back(new Bullet(m_pPlayer->m_rotationAngle, m_pPlayer->getTransform()->position, true));
			addChild(m_pBullet[TotalBullets]);
			TotalBullets++;
			SoundManager::Instance().playSound("sht", 0, -1);
		}
	}

	////Enemy BulletShooting
	//if (m_pPlayer->isEnabled() == true)
	//{
	//	for (int i = 0; i < 8; i++)
	//	{
	//		if((m_pEnemy[i]->getTransform()->position.x>0.0f) && (m_pEnemy[i]->getTransform()->position.x <800.0f))
	//		{
	//			if ((m_pEnemy[i]->getTransform()->position.y > 0.0f) && (m_pEnemy[i]->getTransform()->position.y < 600.0f))
	//			{
	//				if (m_pEnemy[i]->isEnabled() == true)
	//				{
	//					//Checking LOS
	//					m_CheckShipLOS(m_pETurret[i]);
	//					if (m_pEnemy[i]->cd > 4.0f)
	//					{
	//						//LOS fire
	//						if(m_pETurret[i]->hasLOS())
	//						{
	//							m_pEnemy[i]->seek=true;
	//							m_pEnemy[i]->cd = 0;
	//							m_pEnemyBullet.push_back(new Bullet(m_pETurret[i]->getRotation(), m_pETurret[i]->getTransform()->position, true));
	//							addChild(m_pEnemyBullet[TotalEBullets]);
	//							TotalEBullets++;
	//						}
	//						//Radius (Probably useless)
	//						if (Util::distance(m_pEnemy[i]->getTransform()->position, m_pPlayer->getTransform()->position) < 150)
	//						{
	//							m_pEnemy[i]->cd = 0;
	//							m_pEnemyBullet.push_back(new Bullet(m_pETurret[i]->getRotation(), m_pETurret[i]->getTransform()->position, true));
	//							addChild(m_pEnemyBullet[TotalEBullets]);
	//							TotalEBullets++;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//m_CheckShipLOS(m_pEnemy[0]);
	//if (m_pEnemy[0]->hasLOS())
	//	std::cout << "Has LOS" << std::endl;
	////Win Condition
	//if(m_pPlayer->isEnabled()==false)
	//{
	//	if(ButtonCD>1)
	//	{
	//		TheGame::Instance()->changeSceneState(LOSE_SCENE);
	//	}
	//}
	//else if(EnemiesDestroyed==8)
	//{
	//	if (ButtonCD > 1)
	//	{
	//		TheGame::Instance()->changeSceneState(WIN_SCENE);
	//	}
	//}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	auto offsetTiles1 = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	auto offsetTiles2 = glm::vec2(Config::TILE_SIZE * 1.0f, Config::TILE_SIZE * 0.5f);
	auto offsetEnemiesDown = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f + 60.0f);
	auto offsetEnemiesUp = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f - 60.0f);
	auto offsetEnemiesRight = glm::vec2(Config::TILE_SIZE * 0.5f+60.0f, Config::TILE_SIZE * 0.5f);
	auto offsetEnemiesLeft = glm::vec2(Config::TILE_SIZE * 0.5f-60.0f, Config::TILE_SIZE * 0.5f);

	//Labels

	const SDL_Color blue = { 0, 0, 255, 255 };
	m_Inst[0] = new Label("Remainding Slimes: 6.          Slimes Killed: 0.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[0]->setEnabled(false);
	m_Inst[0]->setParent(this);
	addChild(m_Inst[0],4);

	m_Inst[1] = new Label("Remainding Slimes: 5.          Slimes Killed: 1.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[1]->setEnabled(false);
	m_Inst[1]->setParent(this);
	addChild(m_Inst[1],4);

	m_Inst[2] = new Label("Remainding Slimes: 4.          Slimes Killed: 2.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[2]->setEnabled(false);
	m_Inst[2]->setParent(this);
	addChild(m_Inst[2],4);

	m_Inst[3] = new Label("Remainding Slimes: 3.          Slimes Killed: 3.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[3]->setEnabled(false);
	m_Inst[3]->setParent(this);
	addChild(m_Inst[3],4);

	m_Inst[4] = new Label("Remainding Slimes: 2.          Slimes Killed: 4.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[4]->setEnabled(false);
	m_Inst[4]->setParent(this);
	addChild(m_Inst[4],4);

	m_Inst[5] = new Label("Remainding Slimes: 1.          Slimes Killed: 5.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[5]->setEnabled(false);
	m_Inst[5]->setParent(this);
	addChild(m_Inst[5],4);

	m_Inst[6] = new Label("Remainding Slimes: 0.          Slimes Killed: 6.", "Consolas",
		20, blue, glm::vec2(400.f, 550.f));
	m_Inst[6]->setEnabled(false);
	m_Inst[6]->setParent(this);
	addChild(m_Inst[6],4);


	//Tiles
	m_buildGrid();

	//Background
	Bg = new TileC("../Assets/grid/Bg.png", "Bg");
	Bg->getTransform()->position.x = 800.0f/2;		
	Bg->getTransform()->position.y = 600.0f/2;
	addChild(Bg,0);

	//Obstacles
	
	m_field[0] = new TileC("../Assets/grid/RiverWide.png","w");
	m_field[0]->getTransform()->position = m_getTile(2, 5)->getTransform()->position+offsetTiles2;
	addChild(m_field[0],1);
	m_pMap.push_back(m_field[0]);

	m_field[1] = new TileC("../Assets/grid/RiverWide.png", "w");
	m_field[1]->getTransform()->position = m_getTile(16, 5)->getTransform()->position + offsetTiles2;
	addChild(m_field[1], 1);
	m_pMap.push_back(m_field[1]);

	m_field[2] = new TileC("../Assets/grid/River120.png", "120");
	m_field[2]->getTransform()->position = m_getTile(7, 5)->getTransform()->position + offsetTiles1;
	addChild(m_field[2], 1);
	m_pMap.push_back(m_field[2]);

	m_field[3] = new TileC("../Assets/grid/gridTree.png", "TreeG");
	m_field[3]->getTransform()->position = m_getTile(14, 10)->getTransform()->position + offsetTiles1;
	addChild(m_field[3], 1);
	m_pMap.push_back(m_field[3]);

	m_field[4] = new TileC("/Assets/grid/River120.png", "120");
	m_field[4]->getTransform()->position = m_getTile(12, 5)->getTransform()->position + offsetTiles1;
	addChild(m_field[4], 2);
	m_pMap.push_back(m_field[4]);

	m_field[5] = new TileC("../Assets/grid/gridTree.png", "TreeG");
	m_field[5]->getTransform()->position = m_getTile(5, 10)->getTransform()->position + offsetTiles1;
	addChild(m_field[5], 2);
	m_pMap.push_back(m_field[5]);

	//ENEMIES
	
	//Enemy0
	m_pEnemy[0] = new Enemy();
	m_pEnemy[0]->getTransform()->position = m_getTile(4, 3)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemy[0],2);
	m_pEnemy[0]->move = true;
	//Hp
	m_pEnemy[0]->setCurrentHp(1);
	Enemy0 = new Hp();
	Enemy0->getTransform()->position = { m_pEnemy[0]->getTransform()->position.x,m_pEnemy[0]->getTransform()->position.y - 40 };
	addChild(Enemy0, 3);

	//Enemy1
	m_pEnemy[1] = new Enemy();
	m_pEnemy[1]->getTransform()->position = m_getTile(15, 3)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemy[1], 2);
	//Hp
	m_pEnemy[1]->setCurrentHp(1);
	Enemy1 = new Hp();
	Enemy1->getTransform()->position = { m_pEnemy[1]->getTransform()->position.x,m_pEnemy[1]->getTransform()->position.y - 40 };
	addChild(Enemy1, 3);

	//Enemy2
	m_pEnemy[2] = new Enemy();
	m_pEnemy[2]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemy[2], 2);
	//Hp
	m_pEnemy[2]->setCurrentHp(2);
	Enemy2[0] = new Hp();
	Enemy2[0]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x,m_pEnemy[2]->getTransform()->position.y - 40 };
	addChild(Enemy2[0], 3);
	Enemy2[1] = new Hp();
	Enemy2[1]->getTransform()->position = { m_pEnemy[2]->getTransform()->position.x + 10,m_pEnemy[2]->getTransform()->position.y - 40 };
	addChild(Enemy2[1], 3);

	//Enemy3
	m_pEnemy[3] = new Enemy();
	m_pEnemy[3]->getTransform()->position = m_getTile(13, 12)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemy[3], 2);
	//Hp
	m_pEnemy[3]->setCurrentHp(2);
	Enemy3[0]= new Hp();
	Enemy3[0]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x,m_pEnemy[3]->getTransform()->position.y - 40 };
	addChild(Enemy3[0], 3);
	Enemy3[1] = new Hp();
	Enemy3[1]->getTransform()->position = { m_pEnemy[3]->getTransform()->position.x + 10,m_pEnemy[3]->getTransform()->position.y - 40 };
	addChild(Enemy3[1], 3);

	//Enemy4
	m_pEnemy[4] = new Enemy();
	m_pEnemy[4]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemy[4], 2);
	//Hp
	m_pEnemy[4]->setCurrentHp(2);
	Enemy4[0] = new Hp();
	Enemy4[0]->getTransform()->position = { m_pEnemy[4]->getTransform()->position.x,m_pEnemy[4]->getTransform()->position.y - 40 };
	addChild(Enemy4[0], 3);
	Enemy4[1] = new Hp();
	Enemy4[1]->getTransform()->position = { m_pEnemy[4]->getTransform()->position.x + 10,m_pEnemy[4]->getTransform()->position.y - 40 };
	addChild(Enemy4[1], 3);
	
	//Enemy5
	m_pEnemy[5] = new Enemy();
	m_pEnemy[5]->getTransform()->position = m_getTile(6, 12)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemy[5], 2);
	//Hp
	m_pEnemy[5]->setCurrentHp(2);
	Enemy5[0] = new Hp();
	Enemy5[0]->getTransform()->position = { m_pEnemy[5]->getTransform()->position.x,m_pEnemy[5]->getTransform()->position.y - 40 };
	addChild(Enemy5[0], 3);
	Enemy5[1] = new Hp();
	Enemy5[1]->getTransform()->position = { m_pEnemy[5]->getTransform()->position.x + 10,m_pEnemy[5]->getTransform()->position.y - 40 };
	addChild(Enemy5[1], 3);

	//Enemy Debug//
	for (int i = 0; i < Enemies; i++)
	{
		m_pEnemyDebug[i] = new EnemyDebugMode(m_pEnemy[i]);
		m_pEnemyDebug[i]->setEnabled(false);
		addChild(m_pEnemyDebug[i], 0);
	}
	
	//PLAYER:
	//PlayerAgent
	m_pPlayer = new PlayerAgent();
	m_pPlayer->getTransform()->position = m_getTile(10,0)->getTransform()->position+offsetTiles1;
	m_pPlayer->setEnabled(true);
	addChild(m_pPlayer,2);
	m_pMap.push_back(m_pPlayer);

	//Player HP
	m_pPlayer->setCurrentHp(2.0f);
	PlayerHp[0] = new Hp();
	PlayerHp[0]->getTransform()->position = { m_pPlayer->getTransform()->position.x,m_pPlayer->getTransform()->position.y - 40 };
	addChild(PlayerHp[0],3);
	PlayerHp[1] = new Hp();
	PlayerHp[1]->getTransform()->position = { m_pPlayer->getTransform()->position.x+ 10,m_pPlayer->getTransform()->position.y - 40 };
	addChild(PlayerHp[1],3);
	PlayerHp[2] = new Hp();
	PlayerHp[2]->getTransform()->position = { m_pPlayer->getTransform()->position.x- 10,m_pPlayer->getTransform()->position.y - 40 };
	addChild(PlayerHp[2],3);

	//DECISION TREES:
	//Enemy 0
	for (int i = 0; i < Enemies; i++)
	{
		decisionTree[i] = new DecisionTree();
		decisionTree[i]->setAgent(m_pEnemy[i]);
	}
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
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	////TODO Enable the seek function after swarming
	////Tank 0
	if (decisionTree[0]->MakeDecision() == "Patrol Action")//8,3-0,3
	{
		float dst0;
		if (m_pEnemy[0]->p0 == false)
		{
			dst0 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(8, 3)->getTransform()->position + offset);
			m_pEnemy[0]->setDestination(m_getTile(8, 3)->getTransform()->position + offset);
			if(dst0<5.0f)
			{
				m_pEnemy[0]->p0 = true;
			}
		}
		if (m_pEnemy[0]->p0 == true)
		{
			m_pEnemy[0]->setDestination(m_getTile(0, 3)->getTransform()->position + offset);
			dst0 = Util::distance(m_pEnemy[0]->getTransform()->position, m_getTile(0, 3)->getTransform()->position + offset);
			if (dst0 < 5.0f)
			{
				m_pEnemy[0]->p0 = false;
			}
		}
		
	}
	//else if(decisionTree[0]->MakeDecision() == "Move To Player Action")
	//	m_pEnemy[0]->setDestination(m_pPlayer->getTransform()->position);
	
	//Tank 1
	if (decisionTree[1]->MakeDecision() == "Patrol Action")//11,3-19,3
	{
		float dst0;
		if (m_pEnemy[1]->p0 == false)
		{
			dst0 = Util::distance(m_pEnemy[1]->getTransform()->position, m_getTile(11, 3)->getTransform()->position + offset);
			m_pEnemy[1]->setDestination(m_getTile(11, 3)->getTransform()->position + offset);
			if (dst0 < 5.0f)
			{
				m_pEnemy[1]->p0 = true;
			}
		}
		if (m_pEnemy[1]->p0 == true)
		{
			m_pEnemy[1]->setDestination(m_getTile(19, 3)->getTransform()->position + offset);
			dst0 = Util::distance(m_pEnemy[1]->getTransform()->position, m_getTile(19, 3)->getTransform()->position + offset);
			if (dst0 < 5.0f)
			{
				m_pEnemy[1]->p0 = false;
			}
		}

	}
	//else if(decisionTree[1]->MakeDecision() == "Move To Player Action")
	//	m_pEnemy[1]->setDestination(m_pPlayer->getTransform()->position);

	////Tank 2
	if (decisionTree[2]->MakeDecision() == "Patrol Action")//12,8/12,12/16,12/16,8
	{
		float dst2;
		if (m_pEnemy[2]->p0 == false)
		{
			dst2 = Util::distance(m_pEnemy[2]->getTransform()->position, m_getTile(12, 8)->getTransform()->position + offset);
			m_pEnemy[2]->setDestination(m_getTile(12, 8)->getTransform()->position + offset);
			if (dst2 < 5.0f)
			{
				m_pEnemy[2]->p0 = true;
			}
		}
		if (m_pEnemy[2]->p0 == true)
		{
			m_pEnemy[2]->setDestination(m_getTile(12, 12)->getTransform()->position + offset);
			dst2 = Util::distance(m_pEnemy[2]->getTransform()->position, m_getTile(12, 12)->getTransform()->position + offset);
			if (dst2 < 5.0f)
			{
				m_pEnemy[2]->p1 = true;
			}
		}
		if ((m_pEnemy[2]->p0 == true) && (m_pEnemy[2]->p1 == true))
		{
			m_pEnemy[2]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
			dst2 = Util::distance(m_pEnemy[2]->getTransform()->position, m_getTile(16, 12)->getTransform()->position + offset);
			if (dst2 < 5.0f)
			{
				m_pEnemy[2]->p2 = true;
			}
		}
		if ((m_pEnemy[2]->p1 == true) && (m_pEnemy[2]->p2 == true))
		{
			m_pEnemy[2]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
			dst2 = Util::distance(m_pEnemy[2]->getTransform()->position, m_getTile(16, 8)->getTransform()->position + offset);
			if (dst2 < 5.0f)
			{
				m_pEnemy[2]->p0 = false;
				m_pEnemy[2]->p1 = false;
				m_pEnemy[2]->p2 = false;
			}
		}
	}
	//else if(decisionTree[2]->MakeDecision() == "Move To Player Action")
	//	m_pEnemy[2]->setDestination(m_pPlayer->getTransform()->position);

	////Tank 3 //
	if (decisionTree[3]->MakeDecision() == "Patrol Action")//16,12/16,8/12,8/12,12
	{
		float dst3;
		if (m_pEnemy[3]->p0 == false)
		{
			dst3 = Util::distance(m_pEnemy[3]->getTransform()->position, m_getTile(16, 12)->getTransform()->position + offset);
			m_pEnemy[3]->setDestination(m_getTile(16, 12)->getTransform()->position + offset);
			if (dst3 < 5.0f)
			{
				m_pEnemy[3]->p0 = true;
			}
		}
		if (m_pEnemy[3]->p0 == true)
		{
			m_pEnemy[3]->setDestination(m_getTile(16, 8)->getTransform()->position + offset);
			dst3 = Util::distance(m_pEnemy[3]->getTransform()->position, m_getTile(16, 8)->getTransform()->position + offset);
			if (dst3 < 5.0f)
			{
				m_pEnemy[3]->p1 = true;
			}
		}
		if ((m_pEnemy[3]->p0 == true) && (m_pEnemy[3]->p1 == true))
		{
			m_pEnemy[3]->setDestination(m_getTile(12, 8)->getTransform()->position + offset);
			dst3 = Util::distance(m_pEnemy[3]->getTransform()->position, m_getTile(12, 8)->getTransform()->position + offset);
			if (dst3 < 5.0f)
			{
				m_pEnemy[3]->p2 = true;
			}
		}
		if ((m_pEnemy[3]->p1 == true) && (m_pEnemy[3]->p2 == true))
		{
			m_pEnemy[3]->setDestination(m_getTile(12, 12)->getTransform()->position + offset);
			dst3 = Util::distance(m_pEnemy[3]->getTransform()->position, m_getTile(12, 12)->getTransform()->position + offset);
			if (dst3 < 5.0f)
			{
				m_pEnemy[3]->p0 = false;
				m_pEnemy[3]->p1 = false;
				m_pEnemy[3]->p2 = false;
			}
		}
	}
	//else if(decisionTree[3]->MakeDecision() == "Move To Player Action")
	//	m_pEnemy[3]->setDestination(m_pPlayer->getTransform()->position);

	//
	////Tank 4
	if (decisionTree[4]->MakeDecision() == "Patrol Action")//7,8/7,12/3,12/3,8/
	{
		float dst4;
		if (m_pEnemy[4]->p0 == false)
		{
			dst4 = Util::distance(m_pEnemy[4]->getTransform()->position, m_getTile(7, 8)->getTransform()->position + offset);
			m_pEnemy[4]->setDestination(m_getTile(7, 8)->getTransform()->position + offset);
			if (dst4 < 5.0f)
			{
				m_pEnemy[4]->p0 = true;
			}
		}
		if (m_pEnemy[4]->p0 == true)
		{
			m_pEnemy[4]->setDestination(m_getTile(7, 12)->getTransform()->position + offset);
			dst4 = Util::distance(m_pEnemy[4]->getTransform()->position, m_getTile(7, 12)->getTransform()->position + offset);
			if (dst4 < 5.0f)
			{
				m_pEnemy[4]->p1 = true;
			}
		}
		if ((m_pEnemy[4]->p0 == true) && (m_pEnemy[4]->p1 == true))
		{
			m_pEnemy[4]->setDestination(m_getTile(3, 12)->getTransform()->position + offset);
			dst4 = Util::distance(m_pEnemy[4]->getTransform()->position, m_getTile(3, 12)->getTransform()->position + offset);
			if (dst4 < 5.0f)
			{
				m_pEnemy[4]->p2 = true;
			}
		}
		if ((m_pEnemy[4]->p1 == true) && (m_pEnemy[4]->p2 == true))
		{
			m_pEnemy[4]->setDestination(m_getTile(3, 8)->getTransform()->position + offset);
			dst4 = Util::distance(m_pEnemy[4]->getTransform()->position, m_getTile(3, 8)->getTransform()->position + offset);
			if (dst4 < 5.0f)
			{
				m_pEnemy[4]->p0 = false;
				m_pEnemy[4]->p1 = false;
				m_pEnemy[4]->p2 = false;
			}
		}
	}
	//else if(decisionTree[4]->MakeDecision() == "Move To Player Action")
	//	m_pEnemy[4]->setDestination(m_pPlayer->getTransform()->position);

	////Tank 5
	if (decisionTree[5]->MakeDecision() == "Patrol Action")//3,12/3,8/7,8/7,12/
	{
		float dst5;
		if (m_pEnemy[5]->p0 == false)
		{
			dst5 = Util::distance(m_pEnemy[5]->getTransform()->position, m_getTile(3, 12)->getTransform()->position + offset);
			m_pEnemy[5]->setDestination(m_getTile(3, 12)->getTransform()->position + offset);
			if (dst5 < 5.0f)
			{
				m_pEnemy[5]->p0 = true;
			}
		}
		if (m_pEnemy[5]->p0 == true)
		{
			m_pEnemy[5]->setDestination(m_getTile(3, 8)->getTransform()->position + offset);
			dst5 = Util::distance(m_pEnemy[5]->getTransform()->position, m_getTile(3, 8)->getTransform()->position + offset);
			if (dst5 < 5.0f)
			{
				m_pEnemy[5]->p1 = true;
			}
		}
		if ((m_pEnemy[5]->p0 == true) && (m_pEnemy[5]->p1 == true))
		{
			m_pEnemy[5]->setDestination(m_getTile(7, 8)->getTransform()->position + offset);
			dst5 = Util::distance(m_pEnemy[5]->getTransform()->position, m_getTile(7, 8)->getTransform()->position + offset);
			if (dst5 < 5.0f)
			{
				m_pEnemy[5]->p2 = true;
			}
		}
		if ((m_pEnemy[5]->p1 == true) && (m_pEnemy[5]->p2 == true))
		{
			m_pEnemy[5]->setDestination(m_getTile(7, 12)->getTransform()->position + offset);
			dst5 = Util::distance(m_pEnemy[5]->getTransform()->position, m_getTile(7, 12)->getTransform()->position + offset);
			if (dst5 < 5.0f)
			{
				m_pEnemy[5]->p0 = false;
				m_pEnemy[5]->p1 = false;
				m_pEnemy[5]->p2 = false;
			}
		}
	}
	//else if(decisionTree[5]->MakeDecision() == "Move To Player Action")
	//	m_pEnemy[5]->setDestination(m_pPlayer->getTransform()->position);
}

void PlayScene::m_CheckShipLOS(NavigationAgent* object)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(object->getTransform()->position, m_pPlayer->getTransform()->position);
	if (ShipToTargetDistance <= object->getLOSDistance())
	{
		std::vector<NavigationAgent*> contactList;
		for (auto obj : m_pMap)
		{
			//Check if object is farther than the target
			auto ShipToObjectDistance = Util::distance(object->getTransform()->position, obj->getTransform()->position);
			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((obj->getType() != object->getType()) && (object->getType() != m_pPlayer->getType()))
				{
					contactList.push_back(obj);
				}
			}
		}
		contactList.push_back(m_pPlayer); //add the target at the end of the list
		auto hasLOS = CollisionManager::LOSCheck(object->getTransform()->position,
			object->getTransform()->position + object->getOrientation() * object->getLOSDistance(),
			contactList, m_pPlayer);
		object->setHasLOS(hasLOS);
	}
}

void PlayScene::m_CheckShipCloseCombatPlayer(NavigationAgent* object)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(m_pPlayer->getTransform()->position, object->getTransform()->position);
	if (ShipToTargetDistance <= m_pPlayer->getCloseCombatDistance())
	{
		std::vector<NavigationAgent*> contactListCloseCombat;
		for (auto obj : m_pMap)
		{
			//Check if object is farther than the target
			auto ShipToObjectDistance = Util::distance(m_pPlayer->getTransform()->position, obj->getTransform()->position);
			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((obj->getType() != m_pPlayer->getType()) && (obj->getType() != object->getType()))
				{
					contactListCloseCombat.push_back(obj);
				}
			}
		}
		contactListCloseCombat.push_back(object); //add the target at the end of the list
		auto hasCloseCombatDistance = CollisionManager::LOSCheck(m_pPlayer->getTransform()->position,
			m_pPlayer->getTransform()->position + m_pPlayer->getOrientation() * m_pPlayer->getCloseCombatDistance(),
			contactListCloseCombat, object);
		m_pPlayer->setIsInCloseCombatDistance(hasCloseCombatDistance);
	}
}
