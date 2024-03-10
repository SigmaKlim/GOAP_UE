#pragma once

#include "Planner.h"

class Helper
{
public:
    Helper(Planner* plannerPtr);
    //input: attributeName->ICondition*
    ConditionSet MakeConditionSet(const std::map<std::string, ICondition*>& input) const;
    //input: attributeName->attributeValue
    ValueSet MakeValueSet(const std::map<std::string, t_value>& input) const;


private:
    Planner* _plannerPtr;
};