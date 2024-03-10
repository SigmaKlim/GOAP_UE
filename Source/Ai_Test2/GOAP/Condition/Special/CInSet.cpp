#include "CInSet.h"
#include "CEqual.h"
#include "CLarger.h"

CInSet::CInSet(const std::unordered_set<t_value>& valueSet): Values(valueSet)
{}

float CInSet::Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const
{
    return (float)(!Values.contains(value)); //we assume that we deal with enum-like attribute ///set of level nodes?????
}

ICondition* CInSet::ResolveCEqual(const CEqual* cEqual) const
{
    return Values.contains(cEqual->Value) ? new CEqual(cEqual->Value) : nullptr;
}

ICondition* CInSet::ResolveCLarger(const CLarger* cLarger) const
{
    std::unordered_set<t_value> goodValues;
    for (auto& value : Values)
        if (value < cLarger->Value)
            goodValues.insert(value);
    return goodValues.empty() ? nullptr : new CInSet(goodValues);
}


ICondition* CInSet::ResolveCInSet(const CInSet* cInSet) const
{
    std::unordered_set<t_value> goodValues;
    for (auto& value : Values)
        if (cInSet->Values.contains(value))
            goodValues.insert(value);
    return goodValues.empty() ? nullptr : new CInSet(goodValues);
}