#pragma once


#include <map>

#include "DataBase.h"
#include "Conditions/Basic/ConditionSet.h"

class Helper
{
public:
    Helper(const DataBase& data) : _data(data) {}


    
    //input: attributeName->ICondition*
    ConditionSet MakeConditionSet(const std::map<std::string, ICondition*>& input) const;
    //input: attributeName->attributeValue
    ValueSet MakeValueSet(const std::map<std::string, t_value>& input) const;
    

private:
    const DataBase& _data;
};