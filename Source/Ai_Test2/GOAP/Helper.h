#pragma once

#include <map>

#include "DataBase.h"
#include "Actions/Performers/IActionPerformer.h"
#include "Conditions/Basic/ConditionSet.h"

class Helper
{
public:
    Helper(DataBase& data) : _data(data) {}

    void RegisterAttribute(const std::string& name, IAttribute* attributePtr) const;
    void RegisterGoal(const std::string& name, Goal* goalPtr) const;
    void RegisterAction(const std::string& name, IAction* actionPtr) const;

    // void InitState(const ValueSet& state);
    // void OverrideState(const std::string& attributeName, t_value value);
    
    //input: attributeName->ICondition*
    ConditionSet MakeConditionSet(const std::map<std::string, ICondition*>& input) const;
    //input: attributeName->attributeValue
    ValueSet MakeValueSet(const std::map<std::string, t_value>& input) const;
    

private:
    DataBase& _data;
};