#pragma once
#include "../../Conditions/Basic/ConditionSet.h"
#include "../../ValueSet.h"
#include "../../SupplementalData.h"

//A struct with evaluated action instance data
struct ActionInstanceData
{
    ConditionSet Conditions;
    ValueSet Effects;
    float Cost;

    SupplementalData UserData; //used for constructing new vertices

    //Debug
    std::string StringData; //some info about action this action instance
};

//An interface with methods for constructing action instances from required conditions and user data
class IAction
{
public:
    virtual ~IAction() = default;

    //Construct action instances (like go to cover1, go to cover2) given the required conditions.
    //The function is called in the process of future->past plan building, so the agent state prior to this action taken is unknown.
    //Hence, in the situation when action effects rely on the previous state, you have to set the effects so that they satisfy
    //required conditions, but add another condition (which is send further the pipeline to the past), the satisfaction of which would
    //grant the possibility of this action taken. For example, if the action 'Heal' increases current hp count (which is unknown) by 20,
    //instead of making effect like 'hp+=20', take the required hp condition, satisfy it, and add another condition 'hp>required_value - 20'.
    //In the end, you will get the recursive sequence of choosing heal action until the initial value for hp satisfies conditions.
    virtual void ConstructActionInstancesPriori(std::vector<ActionInstanceData>& actionInstances, const ConditionSet& requiredConditions, const SupplementalData& userData) = 0;

    //In contrast with the previous function, this method is called after the plan was build in order to evaluate plan true action data.
    //It is iterated past->future, hence, we know each action previous state. 
    virtual ActionInstanceData ConstructActionInstancePosteriori(const ValueSet& prevState, const ActionInstanceData& prioriActionInstance)
    {
        return prioriActionInstance;
    }
    //Returns maximal possible cost for this action
    virtual float GetMaxCost() const = 0;

    static const DataBase* DataPtr;

};

