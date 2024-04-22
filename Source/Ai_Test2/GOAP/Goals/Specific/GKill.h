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
	ValueSet OnGoalTaken(const ValueSet& resultState) override
	{
		auto overridenState = Goal::OnGoalTaken(resultState);
		if (overridenState.GetValue(DataPtr->GetAttributeId("AEnemyStatus")) == (int)EAVEnemyStatus::eAttacking)
			overridenState.SetValue(DataPtr->GetAttributeId("AEnemyStatus"), (int)EAVEnemyStatus::eInCombatRadius);
		return overridenState;
	}
};