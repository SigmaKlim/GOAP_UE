#pragma once
#include "../Basic/Goal.h"

class GTest : public Goal
{
public:
    GTest(const ConditionSet& goalConditions, float initPriority = 0.0f) : Goal(goalConditions), _priority(initPriority){}
private:
    float _priority;
};