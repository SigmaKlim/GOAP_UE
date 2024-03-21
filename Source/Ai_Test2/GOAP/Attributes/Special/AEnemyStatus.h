#pragma once
#include "../Basic/AEnum.h"
#include <vector>
#include <string>

enum EAVEnemyStatus
{
    eNonVisible,
    eVisible,
    eInRangedCombatRadius,
    eInCloseCombatRadius,
    eAttacking
};

const std::vector<std::string> ES_ENUMERATORS =
    {"NON_VISIBLE",
    "VISIBLE",
    "IN_RANGED_COMBAT_RADIUS",
    "IN_CLOSE_COMBAT_RADIUS",
    "ATTACKING"
    };

class AEnemyStatus : public AEnum
{
public:
    AEnemyStatus() : AEnum(ES_ENUMERATORS) {}
};