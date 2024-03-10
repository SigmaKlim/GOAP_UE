#pragma once

#include "IActionConstructor.h"

//Construct single action with predefined condition and effects
class ACSimple : public IActionConstructor
{
public:
    ACSimple(const ConditionSet& conditions, const ValueSet& effects, float cost) :
    _conditions(conditions), _effects(effects), _cost(cost)   {}
    void ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override
    {
        actions.push_back({_conditions, _effects, _cost, userData});
    }
    float GetMaxCost() const override
    {
        return _cost;
    }
private:
    ConditionSet _conditions;
    ValueSet _effects;
    float _cost;
};