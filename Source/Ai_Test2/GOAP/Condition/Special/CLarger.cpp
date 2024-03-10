#include "CLarger.h"
#include "CEqual.h"
#include "CInSet.h"

CLarger::CLarger(t_value value): Value(value){}

float CLarger::Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const
{
    return value > Value ? 0.0f : attributePtr->GetDifference(value, Value, userData);
}

ICondition* CLarger::ResolveCLarger(const CLarger* cLarger) const
{
    const t_value& a = Value;
    const t_value& b = cLarger->Value;
    if (a > b)
        return new CLarger(a);
    return new CLarger(b);
}

ICondition* CLarger::ResolveCEqual(const CEqual* cEqual) const
{
    if (cEqual->Value > Value)
        return new CEqual(Value);
    return nullptr;
}

ICondition* CLarger::ResolveCInSet(const CInSet* cInSet) const
{
    return cInSet->ResolveCLarger(this);
}