#pragma once
#include "../Basic/IAction.h"
#include "../../DataBase.h"
#include "../../Conditions/Special/CEqual.h"
#include "../../../Public/NavigatorN.h"
#include "../../Attributes/Special/AHealth.h"


class AcHeal : public IAction
{
public:
	AcHeal(size_t iHpLeft, size_t iAtNode, float cost) : IAction(), _iHpLeft(iHpLeft), _iAtNode(iAtNode), _cost(cost) {}
	virtual void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
	ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance) override;
	float GetMaxCost() const override;

    float CalculateCost(int healthStationId) const;
private:
	size_t _iHpLeft;
	size_t _iAtNode;
    float _cost;

    const float UNSAFE_PENALTY = 15.0f; // added to the cost of heal action if the healing station is visible to enemy
};

inline void AcHeal::ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iHpLeft) == true)
    {
        ConditionSet cs(DataPtr->GetNumAttributes());
        ValueSet vs(DataPtr->GetNumAttributes());
        vs.SetValue(_iHpLeft, MAX_HEALTH);
        for (auto& healthStationId : DataPtr->Navigator->GetNodesByTag(EInterestTag::eHealthStation))
        {
            cs.SetCondition(_iAtNode, new CEqual(healthStationId));
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
    return _cost + UNSAFE_PENALTY;
}

inline float AcHeal::CalculateCost(int healthStationId) const
{
    const AInterestPoint* healthStationPtr = DataPtr->Navigator->GetNodeObject(healthStationId);
    if (healthStationPtr->IsVisibleToPlayer())
        return _cost + UNSAFE_PENALTY;
    return _cost;
}
