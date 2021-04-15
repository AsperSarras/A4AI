#include "TiledLevel.h"
#include "TextureManager.h"


TileC::TileC(std::string texture, std::string key)
{
	TextureManager::Instance()->load(texture,key);
	m_Texture = texture;
	m_key = key;

	auto size = TextureManager::Instance()->getTextureSize(key);
	setWidth(size.x);
	setHeight(size.y);
	setType(STAGE);
}

void TileC::draw()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	TextureManager::Instance()->draw(m_key, getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

//void TileC::setString(std::string texture)
//{
//	m_Texture = texture;
//}

//
TiledLevel::TiledLevel(const unsigned short column, const unsigned short row, 
                       const char* tileData, const char* levelData, const char* tileKey) :m_row(row), m_col(column), m_tileKey(tileKey)
{
	auto size = Config::TILE_SIZE;
	//std::ifstream inFile(tileData);
	//if(inFile.is_open())
	//{
	//	char key;
	//	int x, y;
	//	bool obs, haz;
	//	while (!inFile.eof())
	//	{
	//		inFile >> key >> x >> y >> obs >> haz;
	//		m_tiles.emplace(key, new TileC({ x * size,y * size, },{0.0f, 0.0f}, obs, haz));
	//	}
	//}
	//inFile.close();
	//inFile.open(levelData);
	//if (inFile.is_open())
	//{
	//	char key;
	//	m_level.resize(m_row); // Important or we cannot use subscripts.
	//	for (unsigned short row = 0; row < m_row; row++)
	//	{
	//		m_level[row].resize(m_col);
	//		for (unsigned short col = 0; col < m_col; col++)
	//		{
	//			inFile >> key;
	//			m_level[row][col] = m_tiles[key]->Clone(); // Common prototype method.
	//			m_level[row][col]->SetXY((float)(col * size), (float)(row * size));
	//			if (m_level[row][col]->IsObstacle())
	//				m_obstacles.push_back(m_level[row][col]);
	//		}
	//	}
	//}
	//inFile.close();
}

TiledLevel::~TiledLevel()
{
	//// Clear the tile clones. This also clears the ones in m_obstacles.
	//for (unsigned short row = 0; row < m_row; row++)
	//{
	//	for (unsigned short col = 0; col < m_col; col++)
	//	{
	//		delete m_level[row][col];
	//		m_level[row][col] = nullptr;
	//	}
	//}
	//m_level.clear();
	m_obstacles.clear();
	// Clear the original tiles.
	for (std::map<char, TileC*>::iterator i = m_tiles.begin(); i != m_tiles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_tiles.clear();
}

void TiledLevel::draw()
{
	for (unsigned short row = 0; row < m_row; row++)
	{
		for (unsigned short col = 0; col < m_col; col++)
		{
			TextureManager::Instance()->draw(m_tileKey,
				getTransform()->position.x, getTransform()->position.y, 0, 255, true);
			//SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_tileKey),
			//	m_level[row][col]->getTransform()->position, m_level[row][col]->getTransform()->scale);
		}
	}
}

DestructibleObstacle::~DestructibleObstacle()
= default;

void DestructibleObstacle::setCurrentHp(int n)
{
	currentHp = n;
}

int DestructibleObstacle::getCurrentHp() const
{
	return currentHp;
}

void DestructibleObstacle::Destroy()
{
	setEnabled(false);
}
