#include "Helper.h"


//
// void Helper::InitState(const ValueSet& state)
// {
//     assert(state.NumAffected() == state.Size());
//     _gController._agentState = state;
// }

// void Helper::OverrideState(const std::string& attributeName, t_value value)
// {
//     size_t aId = _gController.GetAttributeId(attributeName);
//     _gController._agentState.SetValue(aId, value);
// }

ConditionSet Helper::MakeConditionSet(const std::map<std::string, ICondition*>& input) const
{
    ConditionSet cs(_data.GetNumAttributes());
    for (auto& pair : input)
    {
        MY_ASSERT(_data.GetAttributeId(pair.first) != SIZE_MAX);
        cs.SetCondition(_data.GetAttributeId(pair.first), pair.second);
    }
    return cs;
}

ValueSet Helper::MakeValueSet(const std::map<std::string, t_value>& input) const
{
    ValueSet vs(_data.GetNumAttributes());
    for (auto& pair : input)
    {
        MY_ASSERT(_data.GetAttributeId(pair.first) != SIZE_MAX);
        vs.SetValue(_data.GetAttributeId(pair.first), pair.second);
    }
    return vs;
}
