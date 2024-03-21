﻿#pragma once
#include "../../Conditions/Basic/ConditionSet.h"
#include "../../DataBase.h"
class Goal
{
public:
    Goal(const ConditionSet& goalConditions) :
    _conditions(goalConditions) {}
    virtual ~Goal() = default;
    //controller redirects user calls of UpdateGoalPriority to this method
    virtual float UpdatePriority() = 0;
    //called after the goal is completed to adjust intermediate state before the next plan is built
    virtual ValueSet OverrideAgentState(const ValueSet& resultState)
    {
        return resultState;
    }
    inline const ConditionSet& GetConditions() const;

    static const DataBase* DataPtr; 
    
protected:
    ConditionSet _conditions;
    
};

inline const ConditionSet& Goal::GetConditions() const
{
    return _conditions;
}
