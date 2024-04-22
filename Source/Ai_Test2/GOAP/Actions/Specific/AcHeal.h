#pragma once
#include "../Basic/IAction.h"
#include "../../DataBase.h"
#include "../../Conditions/Special/CEqual.h"
#include "../../../Public/NavigatorN.h"
#include "../../Attributes/Special/AHealth.h"

class AcHeal : public IAction
{
public:
	AcHeal(size_t iHpLeft, size_t iAtNode) : IAction(), _iHpLeft(iHpLeft), _iAtNode(iAtNode) {}
	virtual void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
	ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance) override;
	float GetMaxCost() const override;
    float CalculateCost(int healthStationId) const;
private:
	size_t _iHpLeft;
	size_t _iAtNode;

    const float penalty = 15.0f; // added to the cost of heal action if the healing station is visible to enemy
};

inline void AcHeal::ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iHpLeft) == true)
    {
        ConditionSet cs(DataPtr->GetNumAttributes());
        for (auto& healthStationId : DataPtr->Navigator->GetNodesByTag(EInterestTag::eHealthStation))
        {
            cs.SetCondition(_iAtNode, new CEqual(healthStationId));
            ValueSet vs(DataPtr->GetNumAttributes());
            vs.SetValue(_iHpLeft, MAX_HEALTH);
            ActionInstanceData action(cs, vs, CalculateCost(healthStationId), userData, "");
            actions.push_back(action);
        }
    }
}

inline ActionInstanceData AcHeal::ConstructActionInstancePosteriori(
    const ValueSet& prevState, const ActionInstanceData& prioriActionInstance)
{
    return ConstructActionInstancePosteriori(prevState, prioriActionInstance);
}

inline float AcHeal::GetMaxCost() const
{
    return DataPtr->Navigator->GetMaxDistance() + penalty;
}

inline float AcHeal::CalculateCost(int healthStationId) const
{
    return 0.0f;
}
