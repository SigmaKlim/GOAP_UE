#pragma once


#include <map>

#include "DataBase.h"
#include "Conditions/Basic/ConditionSet.h"

class Helper
{
public:
    Helper(DataBase& data) : _data(data) {}

    bool RegisterAttribute(const std::string& name, IAttribute* attributePtr) const;
    bool RegisterGoal(const std::string& name, Goal* goalPtr) const;
    bool RegisterAction(const std::string& name, IAction* actionPtr) const;


    
    //input: attributeName->ICondition*
    ConditionSet MakeConditionSet(const std::map<std::string, ICondition*>& input) const;
    //input: attributeName->attributeValue
    ValueSet MakeValueSet(const std::map<std::string, t_value>& input) const;
    

private:
    DataBase& _data;
};