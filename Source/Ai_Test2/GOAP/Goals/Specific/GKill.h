#pragma once
#include "../Basic/Goal.h"
#include "../../Attributes/Special/AEnemyStatus.h"
#include "../../Conditions/Special/CEqual.h"

class GKill : public Goal
{
public:
	GKill() 
	{
		_conditions.SetCondition(DataPtr->GetAttributeId("AEnemyStatus"), new CEqual((int)EAVEnemyStatus::eAttacking));
		
	}
	ValueSet OverrideAgentState(const ValueSet& resultState) override
	{
		auto overridenState(resultState);
		overridenState.SetValue(DataPtr->GetAttributeId("AEnemyStatus"), (int)EAVEnemyStatus::eInCombatRadius);
		return overridenState;
	}
};