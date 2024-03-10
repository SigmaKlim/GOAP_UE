#pragma once
#include "../Basic/IActionConstructor.h"
#include "../../Condition/Special/CLarger.h"

class ACUseDepletable : public IActionConstructor
{
public:
    ACUseDepletable(size_t iNumDepletables, size_t iAffectedAttribute, t_value delta, float cost,
        const std::string& depletableName = "depletable", const std::string& affectedAttributeName = "attribute") :
    _iNumDepletables(iNumDepletables), _iAffectedAttribute(iAffectedAttribute), _delta(delta), _cost(cost),
    _depletableName(depletableName), _affectedAttributeName(affectedAttributeName) {}
    
    void ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override;
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

inline void ACUseDepletable::ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions,
    const SupplementalData& userData)
{
    if (requiredConditions.IsAffected(_iAffectedAttribute) == true)
    {
        int minNumDepletables = 0;
        if (requiredConditions.IsAffected(_iNumDepletables) == true)
        {
            auto* cLargerNumDepletables = static_cast<const CLarger*>(requiredConditions.GetProperty(_iNumDepletables).get());
            minNumDepletables = cLargerNumDepletables->Value + 1; //Make minimal required number of health kits larger by 1, as this action depletes 1 kit
        }
        ConditionSet cs(numAttributes);
        cs.SetCondition(_iNumDepletables, new CLarger(minNumDepletables));
        auto* cLargerAffected = static_cast<const CLarger*>(requiredConditions.GetProperty(_iAffectedAttribute).get());
        t_value requiredValue = cLargerAffected->Value;
        cs.SetCondition(_iAffectedAttribute, new CLarger(requiredValue - _delta)); //Make minimal required value of the affected attribute less by delta, as this action increases it by delta
        ValueSet vs(numAttributes);
        vs.SetValue(_iAffectedAttribute, requiredValue);
        std::string stringData = "(+" + std::to_string(_delta) + " " + _affectedAttributeName + "); req. " + _depletableName + "s: " + std::to_string(minNumDepletables);
        Action action(cs, vs, _cost, userData, stringData);
        actions.push_back(action);
    }
}

inline float ACUseDepletable::GetMaxCost() const
{
    return _cost;
}
