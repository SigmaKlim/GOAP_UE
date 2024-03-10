#include "CEqual.h"
#include "CInSet.h"
#include "CLarger.h"
CEqual::CEqual(t_value value) : Value(value) {}

float CEqual::Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const
{
    return attributePtr->GetDifference(value, Value, userData);
}

ICondition* CEqual::ResolveCEqual(const CEqual* cEqual) const
{
    return Value == cEqual->Value ? new CEqual(Value) : nullptr;
}

ICondition* CEqual::ResolveCLarger(const CLarger* cLarger) const
{
    return cLarger->ResolveCEqual(this);
}

ICondition* CEqual::ResolveCInSet(const CInSet* cInSet) const
{
    return cInSet->ResolveCEqual(this);
}
