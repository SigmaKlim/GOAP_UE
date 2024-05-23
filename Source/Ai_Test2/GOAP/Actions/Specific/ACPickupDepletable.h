#pragma once
#include <unordered_set>

#include "../../Helper.h"
#include "../../Conditions/Special/CInSet.h"
#include "..\..\Conditions\Special\CGreater.h"
#include "../Basic/IAction.h"

class AcPickupDepletable : public IAction
{
public:
    AcPickupDepletable(size_t iNumPickups, size_t iAtNode,
        const std::unordered_set<int>& pickupLocationNodeIds, float cost, const std::string& depletableName = "depletable") :
    _iNumDepletables(iNumPickups), _iAtNode(iAtNode),
    _pickupLocationNodeIds(pickupLocationNodeIds), _cost(cost), _depletableName(depletableName) {}

    void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
    ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance) override;
    float GetMaxCost() const override;
    
private:
    size_t _iNumDepletables;
    size_t _iAtNode;
    std::unordered_set<int> _pickupLocationNodeIds;
    float _cost;
    //debug
    std::string _depletableName;
};

inline void AcPickupDepletable::ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iNumDepletables) == true)
    {
        ConditionSet cs(DataPtr->GetNumAttributes());
        cs.SetCondition(_iAtNode, new CInSet(_pickupLocationNodeIds));
        auto* greaterPtr = static_cast<const CGreater*>(requiredConditions.GetProperty(_iNumDepletables).get());
        int requiredNumDepletables = greaterPtr->Value + 1; //minimal desired value
        if (requiredNumDepletables > 0)
            cs.SetCondition(_iNumDepletables, new CGreater(requiredNumDepletables - 2)); //reduce required number of depletables by 1
        ValueSet vs(DataPtr->GetNumAttributes());
        vs.SetValue(_iNumDepletables, requiredNumDepletables);
        actions.push_back({cs, vs, _cost, userData, ""});
    }
}

inline ActionInstanceData AcPickupDepletable::ConstructActionInstancePosteriori(
    const ValueSet& prevState, const ActionInstanceData& prioriActionInstance)
{
    auto posterioriActionInstance = IAction::ConstructActionInstancePosteriori(prevState, prioriActionInstance);
    t_value prevNumDepletables = prevState.GetValue(_iNumDepletables); //number of items before the pickup
    posterioriActionInstance.Effects.SetValue(_iNumDepletables, prevNumDepletables + 1);
    posterioriActionInstance.StringData = "(+1 " + _depletableName + "; " + _depletableName + "s left: " + std::to_string(prevNumDepletables + 1) + ")  ";
    return posterioriActionInstance;
}

inline float AcPickupDepletable::GetMaxCost() const
{
    return _cost;
}
