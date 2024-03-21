#pragma once
#include "../Basic/IAction.h"
#include "..\..\Conditions\Special\CGreater.h"

class ACUseDepletable : public IAction
{
public:
    ACUseDepletable(size_t iNumDepletables, size_t iAffectedAttribute, t_value delta, float cost,
        const std::string& depletableName = "depletable", const std::string& affectedAttributeName = "attribute") :
    _iNumDepletables(iNumDepletables), _iAffectedAttribute(iAffectedAttribute), _delta(delta), _cost(cost),
    _depletableName(depletableName), _affectedAttributeName(affectedAttributeName) {}
    
    void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
    ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance) override;
    float GetMaxCost() const override;
    
private:
    size_t _iNumDepletables;   //index of the attribute showing the minimal required number of depletables
    size_t _iAffectedAttribute; //index of the attribute which value is affected by using the depletable (medkit, ammo)
    t_value _delta;             //hpLeft += delta
    float   _cost;
    //debug
    std::string _depletableName;
    std::string _affectedAttributeName;
};

inline void ACUseDepletable::ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iAffectedAttribute) == true)
    {
        int minNumDepletables = 0;
        if (requiredConditions.IsAffected(_iNumDepletables) == true)
        {
            auto* cGreaterNumDepletables = static_cast<const CGreater*>(requiredConditions.GetProperty(_iNumDepletables).get());
            minNumDepletables = cGreaterNumDepletables->Value + 1; //Make minimal required number of health kits Greater by 1, as this action depletes 1 kit
        }
        ConditionSet cs(DataPtr->GetNumAttributes());
        cs.SetCondition(_iNumDepletables, new CGreater(minNumDepletables));
        auto* cGreaterAffected = static_cast<const CGreater*>(requiredConditions.GetProperty(_iAffectedAttribute).get());
        t_value requiredValue = cGreaterAffected->Value + 1;
        cs.SetCondition(_iAffectedAttribute, new CGreater(requiredValue - _delta - 1)); //Make minimal required value of the affected attribute less by delta, as this action increases it by delta
        ValueSet vs(DataPtr->GetNumAttributes());
        vs.SetValue(_iAffectedAttribute, requiredValue);
        ActionInstanceData action(cs, vs, _cost, userData, "");
        actions.push_back(action);
    }
}

inline ActionInstanceData ACUseDepletable::ConstructActionInstancePosteriori(
    const ValueSet& prevState, const ActionInstanceData& prioriActionInstance)
{
    auto posterioriActionInstance = IAction::ConstructActionInstancePosteriori(prevState, prioriActionInstance);
    t_value prevNumDepletables = prevState.GetValue(_iNumDepletables);
    t_value prevAffectedValue = prevState.GetValue(_iAffectedAttribute);
    posterioriActionInstance.Effects.SetValue(_iNumDepletables, prevNumDepletables - 1);
    posterioriActionInstance.Effects.SetValue(_iAffectedAttribute, prevAffectedValue + _delta);
    posterioriActionInstance.StringData = "(+" + std::to_string(_delta) + " " + _affectedAttributeName + ";  " + _depletableName + "s left: " + std::to_string(prevNumDepletables - 1) + ")";
    return posterioriActionInstance;
}

inline float ACUseDepletable::GetMaxCost() const
{
    return _cost;
}
