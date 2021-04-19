#include "RangedCombatEnemy.h"

#include "TextureManager.h"

RangedCombatEnemy::RangedCombatEnemy(std::string texture, std::string key) :Enemy(texture, key)
{
	TextureManager::Instance()->load(texture, key);
	m_key = key;
	setRangedCombatDistance(200.0f);
	setDetectionRadius(225.0f);
}
