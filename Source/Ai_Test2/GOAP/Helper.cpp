#include "Helper.h"

Helper::Helper(Planner* plannerPtr) : _plannerPtr(plannerPtr) {}

ConditionSet Helper::MakeConditionSet(const std::map<std::string, ICondition*>& input) const
{
    ConditionSet cs(_plannerPtr->GetNumAttributes());
    for (auto& pair : input)
    {
        cs.SetCondition(_plannerPtr->GetAttributeId(pair.first), pair.second);
    }
    return cs;
}

ValueSet Helper::MakeValueSet(const std::map<std::string, t_value>& input) const
{
    ValueSet vs(_plannerPtr->GetNumAttributes());
    for (auto& pair : input)
        vs.SetValue(_plannerPtr->GetAttributeId(pair.first), pair.second);
    return vs;
}
