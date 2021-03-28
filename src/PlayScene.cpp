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

	//TextureManager::Instance()->load("../Assets/grid/Bg.png", "Bg");
	//TextureManager::Instance()->load("../Assets/textures/Tiles.png", "tiles");
	//SoundManager::Instance().load("../Assets/audio/Bgm.mp3", "Bgm", SOUND_MUSIC);
	//SoundManager::Instance().load("../Assets/audio/Exp.wav", "Expl", SOUND_SFX);
	//SoundManager::Instance().load("../Assets/audio/Goal.ogg", "Goal", SOUND_SFX);
	//SoundManager::Instance().playMusic("Bgm", -1, 0);
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
	//Player CloseCombat
	m_CheckShipCloseCombatPlayer(m_pEnemyTank[0]);
	//Enemy trees TODO
	m_CheckShipLOS(m_pEnemyTank[0]);
	if (CheckCD > 0.5f)
	{
		std::cout << "---------------------------------" << std::endl;
		std::cout << decisionTree[0]->MakeDecision() << std::endl;
		std::cout << "---------------------------------\n" << std::endl;
		CheckCD = 0;
	}
	if(decisionTree[0]->MakeDecision() == "Patrol Action")
	{
		std::cout << "CONCHETUMARE" << std::endl;
	}
	else if (decisionTree[0]->MakeDecision() == "Move To Player Action")
	{
		std::cout << "QWEA" << std::endl;
	}

	//Enemy movevents
	m_move();

	//Timer for Cooldowns
	GameTimer += 1 * deltaTime;
	ButtonCD += 1 * deltaTime;
	CheckCD += 1 * deltaTime;
	//std::cout << ButtonCD << std::endl;

	//GunCD += 1 * deltaTime;
	//for (auto i = 0; i < 8; i++)
	//{
	//	m_pEnemyTank[i]->cd += 1 * deltaTime;
	//	m_pEnemyTank[i]->avocd += 1 * deltaTime;
	//}
	//if ((m_pPlayerTank->isEnabled() == false)||(EnemiesDestroyed==8))
	//{
	//	ButtonCD += 1 * deltaTime;
	//}
	//std::cout << GameTimer << std::endl;

	//Set Enemy turret destination
	//for (int i = 0; i < 8; i++)
	//{
	//
	//	m_pETurret[i]->setDestination(m_pPlayerTurret->getTransform()->position);
	//}

	//destination
	//m_pEnemyTank[0]->setDestination(m_pPlayerTank->getTransform()->position);
	
	//Enemies turret bind
	m_pETurret[0]->getTransform()->position = { m_pEnemyTank[0]->getTransform()->position.x,m_pEnemyTank[0]->getTransform()->position.y - 40.0f };

	//Enemies hp bind
	//Enemy 0
	Enemy0->getTransform()->position = { m_pEnemyTank[0]->getTransform()->position.x,m_pEnemyTank[0]->getTransform()->position.y - 40 };
	//Enemy 1
	Enemy1->getTransform()->position = { m_pEnemyTank[1]->getTransform()->position.x,m_pEnemyTank[1]->getTransform()->position.y - 40 };
	//Enemy 2
	Enemy2[0]->getTransform()->position = { m_pEnemyTank[2]->getTransform()->position.x,m_pEnemyTank[2]->getTransform()->position.y - 40 };
	Enemy2[1]->getTransform()->position = { m_pEnemyTank[2]->getTransform()->position.x + 10,m_pEnemyTank[2]->getTransform()->position.y - 40 };
	//Enemy 3
	Enemy3[0]->getTransform()->position = { m_pEnemyTank[3]->getTransform()->position.x,m_pEnemyTank[3]->getTransform()->position.y - 40 };
	Enemy3[1]->getTransform()->position = { m_pEnemyTank[3]->getTransform()->position.x + 10,m_pEnemyTank[3]->getTransform()->position.y - 40 };
	//Enemy 4
	Enemy4[0]->getTransform()->position = { m_pEnemyTank[4]->getTransform()->position.x,m_pEnemyTank[4]->getTransform()->position.y - 40 };
	Enemy4[1]->getTransform()->position = { m_pEnemyTank[4]->getTransform()->position.x + 10,m_pEnemyTank[4]->getTransform()->position.y - 40 };
	//Enemy 5
	Enemy5[0]->getTransform()->position = { m_pEnemyTank[5]->getTransform()->position.x,m_pEnemyTank[5]->getTransform()->position.y - 40 };
	Enemy5[1]->getTransform()->position = { m_pEnemyTank[5]->getTransform()->position.x + 10,m_pEnemyTank[5]->getTransform()->position.y - 40 };
	
	//Player hp bind
	PlayerHp[0]->getTransform()->position = { m_pPlayerTank->getTransform()->position.x,m_pPlayerTank->getTransform()->position.y - 40 };
	PlayerHp[1]->getTransform()->position = { m_pPlayerTank->getTransform()->position.x + 10,m_pPlayerTank->getTransform()->position.y - 40 };
	PlayerHp[2]->getTransform()->position = { m_pPlayerTank->getTransform()->position.x - 10,m_pPlayerTank->getTransform()->position.y - 40 };
	//for (int i = 0; i < 8; i++)
	//{
	//	m_pETurret[i]->getTransform()->position = m_pEnemyTank[i]->getTransform()->position;
	//}

	//Player Turret Bind
	//m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;

	//Set Player turret destiantion
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	m_pPlayerTurret->setDestination(glm::vec2(mx,my));

	//Player Bullet Off Screen
	//for (int i = 0; i < m_pBullet.size(); i++)
	//{
	//	if (m_pBullet[i]->getTransform()->position.x >= 800.0f ||
	//		m_pBullet[i]->getTransform()->position.x <= 0.0f ||
	//		m_pBullet[i]->getTransform()->position.y >= 600.0f ||
	//		m_pBullet[i]->getTransform()->position.y <= 0)
	//	{
	//		m_pBullet[i]->setEnabled(false);
	//		break;
	//	}
	//}

	//Collisions

	//Player and stage Collision

	//TODO Manage to set speed back after collision. Properly
	//if(m_pPlayerTank->isEnabled())
	//{
	//	for(int i=0;i<12;i++)
	//	{
	//		if (CollisionManager::CircleAABBTanks(m_pPlayerTank,m_field[i]))
	//		{
	//			m_pPlayerTank->wCollision();
	//		}
	//	}
	//}
	
	//Player and enemy Collision
	//for (int EnemyTanks = 0; EnemyTanks < 8; EnemyTanks++)
	//{
	//	if (m_pEnemyTank[EnemyTanks]->isEnabled() == true)
	//	{
	//		if (m_pPlayerTank->isEnabled() == true)
	//		{
	//			if (CollisionManager::CircleAABBTanks(m_pPlayerTank, m_pEnemyTank[EnemyTanks]))
	//			{
	//				m_pPlayerTank->setEnabled(false);
	//				m_pPlayerTurret->setEnabled(false);
	//				m_pEnemyTank[EnemyTanks]->setEnabled(false);
	//				m_pETurret[EnemyTanks]->setEnabled(false);
	//				SoundManager::Instance().playSound("Expl", 0, -1);
	//			}
	//		}
	//	}
	//}

	//Player bullet and enemy tank collision
	//for (int i = 0; i < m_pBullet.size(); i++)
	//{
	//	for (int y = 0; y < 8; y++)
	//	{
	//		if (m_pBullet[i]->isEnabled())
	//		{
	//			if ((m_pEnemyTank[y]->getTransform()->position.x > 0.0f) && (m_pEnemyTank[y]->getTransform()->position.x < 800.0f))
	//			{
	//				if ((m_pEnemyTank[y]->getTransform()->position.y > 0.0f) && (m_pEnemyTank[y]->getTransform()->position.y < 600.0f))
	//				{
	//					if (m_pEnemyTank[y]->isEnabled() == true)
	//					{
	//						if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_pEnemyTank[y]))
	//						{
	//							m_pBullet[i]->setEnabled(false);
	//							m_pEnemyTank[y]->setEnabled(false);
	//							m_pETurret[y]->setEnabled(false);
	//							EnemiesDestroyed++;
	//							SoundManager::Instance().playSound("Expl", 0, -1);
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//Player bullet and Stage collision
	//for (int i = 0; i < m_pBullet.size(); i++)
	//{
	//	for (int y = 0; y < 12; y++)
	//	{
	//		if (m_pBullet[i]->isEnabled())
	//		{
	//			if (CollisionManager::CircleAABBTanks(m_pBullet[i], m_field[y]))
	//			{
	//				m_pBullet[i]->setEnabled(false);
	//				SoundManager::Instance().playSound("Expl", 0, -1);
	//			}
	//		}
	//	}
	//	
	//}
	
	//Enemy Bullet and player Tank Collision
	//if (m_pPlayerTank->isEnabled() == true)
	//{
	//	for (int i = 0; i < m_pEnemyBullet.size(); i++)
	//	{
	//		if (m_pEnemyBullet[i]->isEnabled())
	//		{
	//			if (CollisionManager::CircleAABBTanks(m_pEnemyBullet[i], m_pPlayerTank))
	//			{
	//				m_pEnemyBullet[i]->setEnabled(false);
	//				m_pPlayerTank->setEnabled(false);
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
	//	if(m_pEnemyTank[EnemyTanks]->seek==true)
	//	{
	//		for (int y = 0; y < 12; y++)
	//		{
	//			//Left whishker
	//			if (CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_LWhishker.Start(),
	//				m_pEnemyTank[EnemyTanks]->m_LWhishker.End(), m_field[y]->getTransform()->position-
	//				glm::vec2(m_field[y]->getWidth()/2, m_field[y]->getHeight()/2),
	//				m_field[y]->getWidth(), m_field[y]->getHeight()))
	//			{
	//				m_pEnemyTank[EnemyTanks]->ColObsL = y;
	//				m_pEnemyTank[EnemyTanks]->tRight = true;
	//				m_pEnemyTank[EnemyTanks]->avoidance = true;
	//			}
	//			if(m_pEnemyTank[EnemyTanks]->ColObsL!=100)
	//			{
	//				if (!(CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_LWhishker.Start(),
	//					m_pEnemyTank[EnemyTanks]->m_LWhishker.End(), m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getTransform()->position -
	//					glm::vec2(m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getWidth() / 2, m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getHeight() / 2),
	//					m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getWidth(), m_field[m_pEnemyTank[EnemyTanks]->ColObsL]->getHeight())))
	//				{
	//					m_pEnemyTank[EnemyTanks]->tRight = false;
	//					m_pEnemyTank[EnemyTanks]->avocd = 0;
	//					if(m_pEnemyTank[EnemyTanks]->avocd>1)
	//					{
	//						m_pEnemyTank[EnemyTanks]->avoidance = false;
	//						m_pEnemyTank[EnemyTanks]->ColObsL = 100;
	//					}
	//				}
	//			}
	//			//Right Whishker
	//			if (CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_RWhishker.Start(),
	//				m_pEnemyTank[EnemyTanks]->m_RWhishker.End(), m_field[y]->getTransform()->position -
	//				glm::vec2(m_field[y]->getWidth() / 2, m_field[y]->getHeight() / 2),
	//				m_field[y]->getWidth(), m_field[y]->getHeight()))
	//			{
	//				m_pEnemyTank[EnemyTanks]->ColObsR = y;
	//				m_pEnemyTank[EnemyTanks]->avoidance = true;
	//				m_pEnemyTank[EnemyTanks]->tLeft = true;
	//			}
	//			if (m_pEnemyTank[EnemyTanks]->ColObsR != 100)
	//			{
	//				if (!(CollisionManager::lineRectCheck(m_pEnemyTank[EnemyTanks]->m_RWhishker.Start(),
	//					m_pEnemyTank[EnemyTanks]->m_RWhishker.End(), m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getTransform()->position -
	//					glm::vec2(m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getWidth() / 2, m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getHeight() / 2),
	//					m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getWidth(), m_field[m_pEnemyTank[EnemyTanks]->ColObsR]->getHeight())))
	//				{
	//					m_pEnemyTank[EnemyTanks]->tLeft = false;
	//					m_pEnemyTank[EnemyTanks]->avocd = 0;
	//					if (m_pEnemyTank[EnemyTanks]->avocd > 1)
	//					{
	//						m_pEnemyTank[EnemyTanks]->avoidance = false;
	//						m_pEnemyTank[EnemyTanks]->ColObsL = 100;
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_H)) //TODO DEBUG
	{
	if(ButtonCD>1)
	{
		if (Debug)
			Debug = false;
		else
		{
			Debug = true;
		}


		ButtonCD = 0;
	}
	m_setGridEnabled(Debug);	
	}
	//Player BulletShooting

	//if (EventManager::Instance().getMouseButton(0) && GunCD > 1)
	//{
	//	if (m_pPlayerTank->isEnabled() == true)
	//	{
	//		GunCD = 0;
	//		m_pBullet.push_back(new Bullet(m_pPlayerTurret->m_rotationAngle, m_pPlayerTurret->getTransform()->position, true));
	//		addChild(m_pBullet[TotalBullets]);
	//		TotalBullets++;
	//	}
	//}

	////Enemy BulletShooting
	//if (m_pPlayerTank->isEnabled() == true)
	//{
	//	for (int i = 0; i < 8; i++)
	//	{
	//		if((m_pEnemyTank[i]->getTransform()->position.x>0.0f) && (m_pEnemyTank[i]->getTransform()->position.x <800.0f))
	//		{
	//			if ((m_pEnemyTank[i]->getTransform()->position.y > 0.0f) && (m_pEnemyTank[i]->getTransform()->position.y < 600.0f))
	//			{
	//				if (m_pEnemyTank[i]->isEnabled() == true)
	//				{
	//					//Checking LOS
	//					m_CheckShipLOS(m_pETurret[i]);
	//					if (m_pEnemyTank[i]->cd > 4.0f)
	//					{
	//						//LOS fire
	//						if(m_pETurret[i]->hasLOS())
	//						{
	//							m_pEnemyTank[i]->seek=true;
	//							m_pEnemyTank[i]->cd = 0;
	//							m_pEnemyBullet.push_back(new Bullet(m_pETurret[i]->getRotation(), m_pETurret[i]->getTransform()->position, true));
	//							addChild(m_pEnemyBullet[TotalEBullets]);
	//							TotalEBullets++;
	//						}
	//						//Radius (Probably useless)
	//						if (Util::distance(m_pEnemyTank[i]->getTransform()->position, m_pPlayerTank->getTransform()->position) < 150)
	//						{
	//							m_pEnemyTank[i]->cd = 0;
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
	//m_CheckShipLOS(m_pEnemyTank[0]);
	//if (m_pEnemyTank[0]->hasLOS())
	//	std::cout << "Has LOS" << std::endl;
	////Win Condition
	//if(m_pPlayerTank->isEnabled()==false)
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
	int hp = 100;
	const SDL_Color blue = { 0, 0, 255, 255 };
	//m_Inst[1] = new Label("Hp:", "Consolas",
	//	20, blue, glm::vec2(200.f, 500.f));
	//m_Inst[1]->setParent(this);
	//addChild(m_Inst[1]);
	//m_Inst[2] = new Label("Pass Through Enemies", "Consolas",
	//	20, blue, glm::vec2(140.f, 520.f));
	//m_Inst[2]->setParent(this);
	//addChild(m_Inst[2]);
	//m_Inst[3] = new Label("ETank Movements:", "Consolas",
	//	20, blue, glm::vec2(110.f, 540.f));
	//m_Inst[3]->setParent(this);
	//addChild(m_Inst[3]);
	//m_Inst[4] = new Label("W=Forward   S=Backwards   A=Rotate Left   D=Rotate Right", "Consolas",
	//	20, blue, glm::vec2(340.f, 560.f));
	//m_Inst[4]->setParent(this);
	//addChild(m_Inst[4]);

	//Tiles
	m_buildGrid();

	//Background TODO Add a good one
	//Bg = new TileC("../Assets/grid/Bg.png", "Bg");
	//Bg->getTransform()->position.x = 800.0f/2;		
	//Bg->getTransform()->position.y = 600.0f/2;
	//addChild(Bg,0);

	//Obstacles
	
	m_field[0] = new TileC("../Assets/grid/wide.png","w");
	m_field[0]->getTransform()->position = m_getTile(2, 5)->getTransform()->position+offsetTiles2;
	addChild(m_field[0],1);
	m_pMap.push_back(m_field[0]);

	m_field[1] = new TileC("../Assets/grid/wide.png", "w");
	m_field[1]->getTransform()->position = m_getTile(16, 5)->getTransform()->position + offsetTiles2;
	addChild(m_field[1], 1);
	m_pMap.push_back(m_field[1]);

	m_field[2] = new TileC("../Assets/grid/120.png", "120");
	m_field[2]->getTransform()->position = m_getTile(7, 5)->getTransform()->position + offsetTiles1;
	addChild(m_field[2], 1);
	m_pMap.push_back(m_field[2]);

	m_field[3] = new TileC("../Assets/grid/120.png", "120");
	m_field[3]->getTransform()->position = m_getTile(12, 5)->getTransform()->position + offsetTiles1;
	addChild(m_field[3], 1);
	m_pMap.push_back(m_field[3]);

	m_field[4] = new TileC("/Assets/grid/120.png", "120");
	m_field[4]->getTransform()->position = m_getTile(14, 10)->getTransform()->position + offsetTiles1;
	addChild(m_field[4], 1);
	m_pMap.push_back(m_field[4]);

	m_field[5] = new TileC("../Assets/grid/120.png", "120");
	m_field[5]->getTransform()->position = m_getTile(5, 10)->getTransform()->position + offsetTiles1;
	addChild(m_field[5], 1);
	m_pMap.push_back(m_field[5]);

	//ENEMIES
	
	//Enemy0
	m_pEnemyTank[0] = new ETank();
	m_pEnemyTank[0]->getTransform()->position = m_getTile(4, 3)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemyTank[0],2);
	//Hp
	m_pEnemyTank[0]->setCurrentHp(0);
	Enemy0 = new Hp();
	Enemy0->getTransform()->position = { m_pEnemyTank[0]->getTransform()->position.x,m_pEnemyTank[0]->getTransform()->position.y - 40 };
	addChild(Enemy0, 3);

	//Enemy1
	m_pEnemyTank[1] = new ETank();
	m_pEnemyTank[1]->getTransform()->position = m_getTile(15, 3)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemyTank[1], 2);
	//Hp
	m_pEnemyTank[1]->setCurrentHp(0);
	Enemy1 = new Hp();
	Enemy1->getTransform()->position = { m_pEnemyTank[1]->getTransform()->position.x,m_pEnemyTank[1]->getTransform()->position.y - 40 };
	addChild(Enemy1, 3);

	//Enemy2
	m_pEnemyTank[2] = new ETank();
	m_pEnemyTank[2]->getTransform()->position = m_getTile(15, 8)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemyTank[2], 2);
	//Hp
	m_pEnemyTank[2]->setCurrentHp(0);
	Enemy2[0] = new Hp();
	Enemy2[0]->getTransform()->position = { m_pEnemyTank[2]->getTransform()->position.x,m_pEnemyTank[2]->getTransform()->position.y - 40 };
	addChild(Enemy2[0], 3);
	Enemy2[1] = new Hp();
	Enemy2[1]->getTransform()->position = { m_pEnemyTank[2]->getTransform()->position.x + 10,m_pEnemyTank[2]->getTransform()->position.y - 40 };
	addChild(Enemy2[1], 3);

	//Enemy3
	m_pEnemyTank[3] = new ETank();
	m_pEnemyTank[3]->getTransform()->position = m_getTile(13, 12)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemyTank[3], 2);
	//Hp
	m_pEnemyTank[3]->setCurrentHp(0);
	Enemy3[0]= new Hp();
	Enemy3[0]->getTransform()->position = { m_pEnemyTank[3]->getTransform()->position.x,m_pEnemyTank[3]->getTransform()->position.y - 40 };
	addChild(Enemy3[0], 3);
	Enemy3[1] = new Hp();
	Enemy3[1]->getTransform()->position = { m_pEnemyTank[3]->getTransform()->position.x + 10,m_pEnemyTank[3]->getTransform()->position.y - 40 };
	addChild(Enemy3[1], 3);

	//Enemy4
	m_pEnemyTank[4] = new ETank();
	m_pEnemyTank[4]->getTransform()->position = m_getTile(4, 8)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemyTank[4], 2);
	//Hp
	m_pEnemyTank[4]->setCurrentHp(0);
	Enemy4[0] = new Hp();
	Enemy4[0]->getTransform()->position = { m_pEnemyTank[4]->getTransform()->position.x,m_pEnemyTank[4]->getTransform()->position.y - 40 };
	addChild(Enemy4[0], 3);
	Enemy4[1] = new Hp();
	Enemy4[1]->getTransform()->position = { m_pEnemyTank[4]->getTransform()->position.x + 10,m_pEnemyTank[4]->getTransform()->position.y - 40 };
	addChild(Enemy4[1], 3);
	
	//Enemy5
	m_pEnemyTank[5] = new ETank();
	m_pEnemyTank[5]->getTransform()->position = m_getTile(6, 12)->getTransform()->position + offsetTiles1;
	addChild(m_pEnemyTank[5], 2);
	//Hp
	m_pEnemyTank[5]->setCurrentHp(0);
	Enemy5[0] = new Hp();
	Enemy5[0]->getTransform()->position = { m_pEnemyTank[5]->getTransform()->position.x,m_pEnemyTank[5]->getTransform()->position.y - 40 };
	addChild(Enemy5[0], 3);
	Enemy5[1] = new Hp();
	Enemy5[1]->getTransform()->position = { m_pEnemyTank[5]->getTransform()->position.x + 10,m_pEnemyTank[5]->getTransform()->position.y - 40 };
	addChild(Enemy5[1], 3);
	
	//m_pEnemyTank[0]->move = true;

	//m_pEnemyTank[1] = new ETank();
	//m_pEnemyTank[1]->getTransform()->position = m_getTile(6, 14)->getTransform()->position + offsetEnemiesDown;
	//addChild(m_pEnemyTank[1], 2);

	//m_pEnemyTank[2] = new ETank();
	//m_pEnemyTank[2]->getTransform()->position = m_getTile(7, 14)->getTransform()->position + offsetEnemiesDown;
	//addChild(m_pEnemyTank[2], 2);

	//m_pEnemyTank[3] = new ETank();
	//m_pEnemyTank[3]->getTransform()->position = m_getTile(0, 7)->getTransform()->position + offsetEnemiesLeft;
	//m_pEnemyTank[3]->setRotation(90.0f);
	//addChild(m_pEnemyTank[3], 2);

	//m_pEnemyTank[4] = new ETank();
	//m_pEnemyTank[4]->getTransform()->position = m_getTile(15, 0)->getTransform()->position + offsetEnemiesUp;
	//m_pEnemyTank[4]->setRotation(180.0f);
	//addChild(m_pEnemyTank[4], 2);

	//m_pEnemyTank[5] = new ETank();
	//m_pEnemyTank[5]->getTransform()->position = m_getTile(19, 8)->getTransform()->position + offsetEnemiesRight;
	//m_pEnemyTank[5]->setRotation(-90.0f);
	//addChild(m_pEnemyTank[5], 2);

	//m_pEnemyTank[6] = new ETank();
	//m_pEnemyTank[6]->getTransform()->position = m_getTile(6, 0)->getTransform()->position + offsetEnemiesUp;
	//m_pEnemyTank[6]->setRotation(180.0f);
	//addChild(m_pEnemyTank[6], 2);

	//m_pEnemyTank[7] = new ETank();
	//m_pEnemyTank[7]->getTransform()->position = m_getTile(19, 6)->getTransform()->position + offsetEnemiesRight;
	//m_pEnemyTank[7]->setRotation(-90.0f);
	//addChild(m_pEnemyTank[7], 2);


	//// Enemy Turret
	m_pETurret[0] = new eTurret();
	m_pETurret[0]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_pETurret[0], 3);

	//m_pETurret[1] = new eTurret();
	//m_pETurret[1]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[1], 3);

	//m_pETurret[2] = new eTurret();
	//m_pETurret[2]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[2], 3);

	//m_pETurret[3] = new eTurret();
	//m_pETurret[3]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[3], 3);

	//m_pETurret[4] = new eTurret();
	//m_pETurret[4]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[4], 3);

	//m_pETurret[5] = new eTurret();
	//m_pETurret[5]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[5], 3);

	//m_pETurret[6] = new eTurret();
	//m_pETurret[6]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[6], 3);

	//m_pETurret[7] = new eTurret();
	//m_pETurret[7]->getTransform()->position = glm::vec2(400.0f, 300.0f);
	//addChild(m_pETurret[7], 3);

	//PLAYER:
	//PlayerTank
	m_pPlayerTank = new PlayerTank();
	m_pPlayerTank->getTransform()->position = m_getTile(10,7)->getTransform()->position+offsetTiles1;
	m_pPlayerTank->setEnabled(true);
	addChild(m_pPlayerTank,2);
	m_pMap.push_back(m_pPlayerTank);
	

	//Player Debug//TODO
	m_pPlayerTurret = new pTurret();
	m_pPlayerTurret->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pPlayerTurret->getTransform()->position = m_pPlayerTank->getTransform()->position;
	addChild(m_pPlayerTurret, 3);

	//Player HP
	m_pPlayerTank->setCurrentHp(2.0f);
	PlayerHp[0] = new Hp();
	PlayerHp[0]->getTransform()->position = { m_pPlayerTank->getTransform()->position.x,m_pPlayerTank->getTransform()->position.y - 40 };
	addChild(PlayerHp[0],3);
	PlayerHp[1] = new Hp();
	PlayerHp[1]->getTransform()->position = { m_pPlayerTank->getTransform()->position.x+ 10,m_pPlayerTank->getTransform()->position.y - 40 };
	addChild(PlayerHp[1],3);
	PlayerHp[2] = new Hp();
	PlayerHp[2]->getTransform()->position = { m_pPlayerTank->getTransform()->position.x- 10,m_pPlayerTank->getTransform()->position.y - 40 };
	addChild(PlayerHp[2],3);

	//DECISION TREES:
	//Enemy 0
	decisionTree[0] = new DecisionTree();
	decisionTree[0]->setAgent(m_pEnemyTank[0]);
	decisionTree[0]->DisplayTree();
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
	//auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	////TODO Enable the seek function after swarming
	////Tank 0

	//if (m_pEnemyTank[0]->seek == true)
	//{
	//	m_pEnemyTank[0]->move = true;
	//	m_pEnemyTank[0]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//
	//else if (m_pEnemyTank[0]->p0 == false && m_pEnemyTank[0]->p1 == false)
	//{
	//	if (GameTimer >= 1)
	//		m_pEnemyTank[0]->move = true;
	//	m_pEnemyTank[0]->setDestination(m_getTile(14, 11)->getTransform()->position + offset);
	//	if (m_pEnemyTank[0]->getTransform()->position == m_getTile(14, 11)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[0]->p0 = true;
	//	}

	//}
	//else if (m_pEnemyTank[0]->p0 == true)
	//{
	//	m_pEnemyTank[0]->move = false;
	//	if (GameTimer >= 14)
	//	{
	//		m_pEnemyTank[0]->move = true;
	//		m_pEnemyTank[0]->setDestination(m_getTile(4, 11)->getTransform()->position + offset);
	//		if (m_pEnemyTank[0]->getTransform()->position == m_getTile(4, 11)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[0]->p0 = false;
	//			m_pEnemyTank[0]->p1 = true;
	//		}
	//	}
	//}
	//else if (m_pEnemyTank[0]->p1 == true)
	//{
	//	m_pEnemyTank[0]->setDestination(m_getTile(4, 5)->getTransform()->position + offset);
	//	if (m_pEnemyTank[0]->getTransform()->position == m_getTile(4, 5)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[0]->p2 = true;
	//		m_pEnemyTank[0]->seek = true;
	//	}
	//}
	//
	////Tank 1
	//if (m_pEnemyTank[1]->seek == true)
	//{
	//	m_pEnemyTank[1]->move = true;
	//	m_pEnemyTank[1]->setDestination(m_pPlayerTank->getTransform()->position);
	//}

	//else if (m_pEnemyTank[1]->p0 == false && m_pEnemyTank[1]->p1 == false)
	//{
	//	m_pEnemyTank[1]->move = true;
	//	m_pEnemyTank[1]->setDestination(m_getTile(6, 10)->getTransform()->position + offset);
	//	if (m_pEnemyTank[1]->getTransform()->position == m_getTile(6, 10)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[1]->p0 = true;
	//	}

	//}
	//else if (m_pEnemyTank[1]->p0 == true)
	//{
	//	m_pEnemyTank[1]->setDestination(m_getTile(4, 10)->getTransform()->position + offset);
	//	if (m_pEnemyTank[1]->getTransform()->position == m_getTile(4, 10)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[1]->p0 = false;
	//		m_pEnemyTank[1]->p1 = true;
	//	}
	//}

	//else if (m_pEnemyTank[1]->p1 == true && m_pEnemyTank[1]->p2 == false)
	//{
	//	m_pEnemyTank[1]->setDestination(m_getTile(4, 4)->getTransform()->position + offset);
	//	if (m_pEnemyTank[1]->getTransform()->position == m_getTile(4, 4)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[1]->p2 = true;
	//	}
	//}
	//else if (m_pEnemyTank[1]->p2 == true)
	//{
	//	m_pEnemyTank[1]->setDestination(m_getTile(9, 4)->getTransform()->position + offset);
	//	if (m_pEnemyTank[1]->getTransform()->position == m_getTile(9, 4)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[1]->p3 = true;
	//		m_pEnemyTank[1]->seek = true;
	//	}
	//}

	////Tank 2
	//m_pEnemyTank[2]->move = true;
	//if (m_pEnemyTank[2]->seek == true)
	//{
	//	m_pEnemyTank[2]->move = true;
	//	m_pEnemyTank[2]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//else if (m_pEnemyTank[2]->move == true)
	//{
	//	if (m_pEnemyTank[2]->p0 == false && m_pEnemyTank[2]->p1 == false)
	//	{
	//		m_pEnemyTank[2]->setDestination(m_getTile(7, 10)->getTransform()->position + offset);
	//		if (m_pEnemyTank[2]->getTransform()->position == m_getTile(7, 10)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[2]->p0 = true;
	//		}

	//	}
	//	else if (m_pEnemyTank[2]->p0 == true)
	//	{
	//		m_pEnemyTank[2]->setDestination(m_getTile(10, 10)->getTransform()->position + offset);
	//		if (m_pEnemyTank[2]->getTransform()->position == m_getTile(10, 10)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[2]->p0 = false;
	//			m_pEnemyTank[2]->p1 = true;

	//		}
	//	}
	//	else if (m_pEnemyTank[2]->p1 == true && m_pEnemyTank[2]->p2 == false)
	//	{
	//		m_pEnemyTank[2]->setDestination(m_getTile(10, 4)->getTransform()->position + offset);
	//		if (m_pEnemyTank[2]->getTransform()->position == m_getTile(10, 4)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[2]->p2 = true;
	//			m_pEnemyTank[2]->seek = true;
	//		}
	//	}
	//}
	//
	////Tank 3 //
	//if (m_pEnemyTank[3]->seek == true)
	//{
	//	m_pEnemyTank[3]->move = true;
	//	m_pEnemyTank[3]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//else if (m_pEnemyTank[3]->p0 == false && m_pEnemyTank[3]->p1 == false)
	//{
	//	if (GameTimer >= 2)
	//		m_pEnemyTank[3]->move = true;
	//	m_pEnemyTank[3]->setDestination(m_getTile(3, 7)->getTransform()->position + offset);
	//	if (m_pEnemyTank[3]->getTransform()->position == m_getTile(3, 7)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[3]->p0 = true;
	//	}

	//}
	//else if (m_pEnemyTank[3]->p0 == true)
	//{
	//	m_pEnemyTank[3]->move = false;
	//	if (GameTimer >= 15)
	//	{
	//		m_pEnemyTank[3]->move = true;
	//		m_pEnemyTank[3]->setDestination(m_getTile(3, 4)->getTransform()->position + offset);
	//		if (m_pEnemyTank[3]->getTransform()->position == m_getTile(3, 4)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[3]->p0 = false;
	//			m_pEnemyTank[3]->p1 = true;
	//			m_pEnemyTank[3]->seek = true;
	//		}
	//	}
	//}

	//
	////Tank 4
	//if (m_pEnemyTank[4]->seek == true)
	//{
	//	m_pEnemyTank[4]->move = true;
	//	m_pEnemyTank[4]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//else if (m_pEnemyTank[4]->p0 == false && m_pEnemyTank[4]->p1 == false)
	//{
	//	if (GameTimer >= 4)
	//		m_pEnemyTank[4]->move = true;
	//	m_pEnemyTank[4]->setDestination(m_getTile(15, 3)->getTransform()->position + offset);
	//	if (m_pEnemyTank[4]->getTransform()->position == m_getTile(15, 3)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[4]->p0 = true;
	//	}

	//}
	//else if (m_pEnemyTank[4]->p0 == true)
	//{
	//	m_pEnemyTank[4]->move = false;
	//	if (GameTimer >= 10)
	//	{
	//		m_pEnemyTank[4]->move = true;
	//		m_pEnemyTank[4]->setDestination(m_getTile(10, 3)->getTransform()->position + offset);
	//		if (m_pEnemyTank[4]->getTransform()->position == m_getTile(10, 3)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[4]->p0 = false;
	//			m_pEnemyTank[4]->p1 = true;
	//			m_pEnemyTank[4]->seek = true;
	//		}
	//	}
	//}
	//
	////Tank 5
	//if (m_pEnemyTank[5]->seek == true)
	//{
	//	m_pEnemyTank[5]->move = true;
	//	m_pEnemyTank[5]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//else if (m_pEnemyTank[5]->p0 == false && m_pEnemyTank[5]->p1 == false)
	//{
	//	if (GameTimer >= 1)
	//		m_pEnemyTank[5]->move = true;
	//	m_pEnemyTank[5]->setDestination(m_getTile(15, 8)->getTransform()->position + offset);
	//	if (m_pEnemyTank[5]->getTransform()->position == m_getTile(15, 8)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[5]->p0 = true;
	//	}

	//}
	//else if (m_pEnemyTank[5]->p0 == true)
	//{
	//	m_pEnemyTank[5]->setDestination(m_getTile(15, 7)->getTransform()->position + offset);
	//	if (m_pEnemyTank[5]->getTransform()->position == m_getTile(15, 7)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[5]->p0 = false;
	//		m_pEnemyTank[5]->p1 = true;
	//	}
	//}
	//else if (m_pEnemyTank[5]->p1 == true)
	//{
	//	m_pEnemyTank[5]->move = false;
	//	if (GameTimer >= 18)
	//	{
	//		m_pEnemyTank[5]->move = true;
	//		m_pEnemyTank[5]->setDestination(m_getTile(15, 9)->getTransform()->position + offset);
	//		if (m_pEnemyTank[5]->getTransform()->position == m_getTile(15, 9)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[5]->p2 = true;
	//		}
	//	}
	//}
	//else if (m_pEnemyTank[5]->p2 == true)
	//{
	//	m_pEnemyTank[5]->setDestination(m_getTile(10, 9)->getTransform()->position + offset);
	//	if (m_pEnemyTank[5]->getTransform()->position == m_getTile(10, 9)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[5]->p3 = true;
	//		m_pEnemyTank[5]->seek = true;
	//	}
	//}

	//
	////Tank 6
	//if (m_pEnemyTank[6]->seek == true)
	//{
	//	m_pEnemyTank[6]->move = true;
	//	m_pEnemyTank[6]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//else if (m_pEnemyTank[6]->p0 == false && m_pEnemyTank[6]->p1 == false)
	//{
	//	if (GameTimer >= 16)
	//		m_pEnemyTank[6]->move = true;
	//	m_pEnemyTank[6]->setDestination(m_getTile(6, 3)->getTransform()->position + offset);
	//	if (m_pEnemyTank[6]->getTransform()->position == m_getTile(6, 3)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[6]->p0 = true;
	//		m_pEnemyTank[6]->seek = true;
	//	}

	//}

	//
	////Tank 7
	//if (m_pEnemyTank[7]->seek == true)
	//{
	//	m_pEnemyTank[7]->move = true;
	//	m_pEnemyTank[7]->setDestination(m_pPlayerTank->getTransform()->position);
	//}
	//else if (m_pEnemyTank[7]->p0 == false && m_pEnemyTank[7]->p1 == false)
	//{
	//	if (GameTimer >= 1)
	//		m_pEnemyTank[7]->move = true;
	//	m_pEnemyTank[7]->setDestination(m_getTile(15, 6)->getTransform()->position + offset);
	//	if (m_pEnemyTank[7]->getTransform()->position == m_getTile(15, 6)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[7]->p0 = true;
	//	}

	//}
	//else if (m_pEnemyTank[7]->p0 == true)
	//{
	//	m_pEnemyTank[7]->setDestination(m_getTile(15, 5)->getTransform()->position + offset);
	//	if (m_pEnemyTank[7]->getTransform()->position == m_getTile(15, 5)->getTransform()->position + offset)
	//	{
	//		m_pEnemyTank[7]->p0 = false;
	//		m_pEnemyTank[7]->p1 = true;
	//	}
	//}
	//else if (m_pEnemyTank[7]->p1 == true)
	//{
	//	m_pEnemyTank[7]->move = false;
	//	if (GameTimer >= 18)
	//	{
	//		m_pEnemyTank[7]->move = true;
	//		m_pEnemyTank[7]->setDestination(m_getTile(14, 5)->getTransform()->position + offset);
	//		if (m_pEnemyTank[7]->getTransform()->position == m_getTile(14, 5)->getTransform()->position + offset)
	//		{
	//			m_pEnemyTank[7]->p2 = true;
	//			m_pEnemyTank[7]->seek = true;
	//		}
	//	}
	//}
}

void PlayScene::m_CheckShipLOS(NavigationAgent* object)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(object->getTransform()->position, m_pPlayerTank->getTransform()->position);
	if (ShipToTargetDistance <= object->getLOSDistance())
	{
		std::vector<NavigationAgent*> contactList;
		for (auto obj : m_pMap)
		{
			//Check if object is farther than the target
			auto ShipToObjectDistance = Util::distance(object->getTransform()->position, obj->getTransform()->position);
			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((obj->getType() != object->getType()) && (object->getType() != m_pPlayerTank->getType()))
				{
					contactList.push_back(obj);
				}
			}
		}
		contactList.push_back(m_pPlayerTank); //add the target at the end of the list
		auto hasLOS = CollisionManager::LOSCheck(object->getTransform()->position,
			object->getTransform()->position + object->getOrientation() * object->getLOSDistance(),
			contactList, m_pPlayerTank);
		object->setHasLOS(hasLOS);
	}
}

void PlayScene::m_CheckShipCloseCombatPlayer(NavigationAgent* object)
{
	// if ship to target distance is less than or equal to LOS distance
	auto ShipToTargetDistance = Util::distance(m_pPlayerTank->getTransform()->position, object->getTransform()->position);
	if (ShipToTargetDistance <= m_pPlayerTank->getCloseCombatDistance())
	{
		std::vector<NavigationAgent*> contactListCloseCombat;
		for (auto obj : m_pMap)
		{
			//Check if object is farther than the target
			auto ShipToObjectDistance = Util::distance(m_pPlayerTank->getTransform()->position, obj->getTransform()->position);
			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((obj->getType() != m_pPlayerTank->getType()) && (obj->getType() != object->getType()))
				{
					contactListCloseCombat.push_back(obj);
				}
			}
		}
		contactListCloseCombat.push_back(object); //add the target at the end of the list
		auto hasCloseCombatDistance = CollisionManager::LOSCheck(m_pPlayerTank->getTransform()->position,
			m_pPlayerTank->getTransform()->position + m_pPlayerTank->getOrientation() * m_pPlayerTank->getCloseCombatDistance(),
			contactListCloseCombat, object);
		m_pPlayerTank->setisInCloseCombatDistance(hasCloseCombatDistance);
	}
}
