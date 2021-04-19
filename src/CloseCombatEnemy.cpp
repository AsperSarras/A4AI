#include "CloseCombatEnemy.h"

#include "TextureManager.h"

CloseCombatEnemy::CloseCombatEnemy(std::string texture, std::string key) :Enemy(texture,key)
{
	TextureManager::Instance()->load(texture, key);
	m_key = key;
	setCloseCombatDistance(60.0f);
	setDetectionRadius(90.0f);
}
