#pragma once
#include "../../Conditions/Basic/ConditionSet.h"

class Goal
{
public:
    Goal();
    Goal(const ConditionSet& goalConditions) :
    _conditions(goalConditions) {}
    virtual ~Goal() = default;
    //Called when the goal is completed. Used to override start state in case the one that was procedurally constructed before is not valid.
    virtual ValueSet OnGoalCompleted(const ValueSet& resultState);
    //Called when the goal is taken. Used to override start state in case the one that was procedurally constructed before is not valid.
    //Please call base method in your overload always.
    virtual ValueSet OnGoalTaken(const ValueSet& resultState);

    virtual float GetTardiness(float priority, int position);
    inline const ConditionSet& GetConditions() const;

    static const DataBase* DataPtr; 
    
protected:
    ConditionSet _conditions;
    
};

inline float Goal::GetTardiness(float priority, int position)
{
    return priority * position;
}

inline const ConditionSet& Goal::GetConditions() const
{
    return _conditions;
}
