#pragma once
#include <unordered_set>

#include "../../Helper.h"
#include "../../Condition/Special/CInSet.h"
#include "../../Condition/Special/CLarger.h"
#include "../Basic/IActionConstructor.h"

class ACPickupDepletable : public IActionConstructor
{
public:
    ACPickupDepletable(size_t iNumPickups, size_t iAtNode,
        const std::unordered_set<int>& pickupLocationNodeIds, float cost, const std::string& depletableName = "depletable") :
    _iNumPickups(iNumPickups), _iAtNode(iAtNode),
    _pickupLocationNodeIds(pickupLocationNodeIds), _cost(cost), _depletableName(depletableName) {}

    void ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
    float GetMaxCost() const override;
    
private:
    size_t _iNumPickups;
    size_t _iAtNode;
    std::unordered_set<int> _pickupLocationNodeIds;
    float _cost;
    //debug
    std::string _depletableName;
};

inline void ACPickupDepletable::ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iNumPickups) == true)
    {
        ConditionSet cs(numAttributes);
        cs.SetCondition(_iAtNode, new CInSet(_pickupLocationNodeIds));
        auto* largerPtr = static_cast<const CLarger*>(requiredConditions.GetProperty(_iNumPickups).get());
        int requiredNumDepletables = largerPtr->Value;
        if (requiredNumDepletables > 0)
            cs.SetCondition(_iNumPickups, new CLarger(requiredNumDepletables - 1)); //reduce required number of depletables by 1
        ValueSet vs(numAttributes);
        vs.SetValue(_iNumPickups, requiredNumDepletables);
        std::string stringData = "(+1 " + _depletableName + "); req. " + _depletableName + "s: " + std::to_string(requiredNumDepletables - 1);
        actions.push_back({cs, vs, _cost, userData, stringData});
    }
}

inline float ACPickupDepletable::GetMaxCost() const
{
    return _cost;
}
