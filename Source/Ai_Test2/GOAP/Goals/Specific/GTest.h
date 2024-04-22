#pragma once
#include "../Basic/Goal.h"

class GTest : public Goal
{
public:
    GTest(const ConditionSet& goalConditions, float initPriority = 0.0f) : Goal(goalConditions), _priority(initPriority){}
    
    //float UpdatePriority() override
    //{
    //    return ++_priority;
    //}

    ValueSet OnGoalTaken(const ValueSet& resultState) override
    {
        return Goal::OnGoalTaken(resultState);
    }
private:
    float _priority;
};