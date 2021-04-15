#pragma once
#ifndef _TILED_LEVEL_
#define _TILED_LEVEL_

#include <fstream>
#include <map>
#include <vector>

#include "NavigationAgent.h"
#include "Tile.h"

class TileC : public Tile
{
public:
	TileC(std::string texture, std::string key);
	bool IsObstacle() { return m_obstacle; }

	void draw() override ;
	void update() override {};
	void clean() override {};

	//void setString(std::string texture);

	
private:
	std::string m_Texture, m_key;
	bool m_obstacle=false;
};

class TiledLevel :public NavigationAgent
{
public:
	TiledLevel(const unsigned short column, const unsigned short row,
		const char* tileData, const char* levelData, const char* tileKey);
	~TiledLevel();

	void draw() override;
	void update() override {};
	void clean() override {};

private:
	const char* m_tileKey;
	int m_row, m_col;
	std::map<char, TileC* > m_tiles;
	//std::vector<std::vector<TileC*>>m_level;
	std::vector<TileC*> m_obstacles;
};

class DestructibleObstacle : public TileC
{
private:
	int currentHp;
public:
	DestructibleObstacle(int hp, std::string texture, std::string key) : TileC(texture, key)
	{
		setType(DESTRUCTABLE);
		currentHp = hp;
	}
	~DestructibleObstacle();
	
	void setCurrentHp(int n);
	int getCurrentHp() const;

	void Destroy();
};

#endif


