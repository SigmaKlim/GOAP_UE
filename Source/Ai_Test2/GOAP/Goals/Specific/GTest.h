#pragma once
#include "../Basic/Goal.h"

class GTest : public Goal
{
public:
    GTest(const ConditionSet& goalConditions, float initPriority = 0.0f) : Goal(goalConditions), _priority(initPriority){}
    
    float UpdatePriority() override
    {
        return ++_priority;
    }

    ValueSet OverrideAgentState(const ValueSet& resultState) override
    {
        auto overridenState = resultState;
        //auto iAtNode = *DataPtr->AttributeCatalogue.GetId("atNode");
        //if (resultState.GetValue(iAtNode) == -1)
        //    overridenState.SetValue(iAtNode, 0);
        return overridenState;
    }
private:
    float _priority;
};