#include "Helper.h"


void Helper::RegisterAttribute(const std::string& name, IAttribute* attributePtr) const
{
    assert(_data.AttributeCatalogue.AddItem(name, std::shared_ptr<IAttribute>(attributePtr)));
}

void Helper::RegisterGoal(const std::string& name, Goal* goalPtr) const
{
    assert(_data.GoalCatalogue.AddItem(name, std::shared_ptr<Goal>(goalPtr)));
}

void Helper::RegisterAction(const std::string& name, IAction* actionPtr) const
{
    assert(_data.ActionCatalogue.AddItem(name, std::shared_ptr<IAction>(actionPtr)));
}
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
        cs.SetCondition(_data.GetAttributeId(pair.first), pair.second);
    }
    return cs;
}

ValueSet Helper::MakeValueSet(const std::map<std::string, t_value>& input) const
{
    ValueSet vs(_data.GetNumAttributes());
    for (auto& pair : input)
        vs.SetValue(_data.GetAttributeId(pair.first), pair.second);
    return vs;
}
