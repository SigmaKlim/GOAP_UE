#pragma once
#include "../Basic/AEnum.h"
#include <vector>
#include <string>

UENUM(BlueprintType)
enum class EAVEnemyStatus : uint8
{
    eNonVisible     UMETA(DisplayName = "Non-visible"),
    eVisible        UMETA(DisplayName = "Visible"),
    eInCombatRadius UMETA(DisplayName = "InCombatRadius"),
    eAttacking      UMETA(DisplayName = "Attacking")
};

const std::vector<std::string> ES_ENUMERATORS =
    {"NON_VISIBLE",
    "VISIBLE",
    "IN_COMBAT_RADIUS",
    "ATTACKING"
    };

class AEnemyStatus : public AEnum
{
public:
    AEnemyStatus() : AEnum(ES_ENUMERATORS) {}
};