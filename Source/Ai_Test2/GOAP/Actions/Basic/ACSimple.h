#pragma once

#include "IAction.h"

//Construct single action with predefined condition and effects
class AcSimple : public IAction
{
public:
    AcSimple(const ConditionSet& conditions, const ValueSet& effects, float cost) :
    _conditions(conditions), _effects(effects), _cost(cost)   {}
    void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) override
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