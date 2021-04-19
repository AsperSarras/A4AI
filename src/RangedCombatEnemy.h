#pragma once

#ifndef __RANGE_COMBAT_ENEMY__
#define __RANGE_COMBAT_ENEMY__

#include "Enemy.h"

class RangedCombatEnemy : public Enemy
{
public:
	RangedCombatEnemy(std::string texture, std::string key);
	~RangedCombatEnemy() = default;
};
#endif // !__RANGE_COMBAT_ENEMY__

