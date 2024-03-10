#pragma once
#include "../../Condition/Basic/ConditionSet.h"
#include "../../ValueSet.h"
#include "../../SupplementalData.h"

//A class for evaluated instances of action
struct Action
{
    ConditionSet Conditions;
    ValueSet Effects;
    float Cost;

    SupplementalData UserData; //used for constructing new vertices

    //Debug
    std::string StringData; //some info about action this action instance
};

//An interface with methods for constructing action instances from required conditions and user data
class IActionConstructor
{
public:
    virtual ~IActionConstructor() = default;
    
    virtual void ConstructActions(std::vector<Action>& actions, const ConditionSet& requiredConditions, const SupplementalData& userData) = 0;

    //Returns maximal possible cost for this action
    virtual float GetMaxCost() const = 0;
    
    static size_t numAttributes;
};

