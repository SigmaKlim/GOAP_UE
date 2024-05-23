#include "CEqual.h"
#include "CInSet.h"
#include "CGreater.h"
CEqual::CEqual(t_value value) : Value(value) {}

float CEqual::Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const
{
    return attributePtr->GetDifference(value, Value, userData);
}

ICondition* CEqual::ResolveCEqual(const CEqual* cEqual) const
{
    return Value == cEqual->Value ? new CEqual(Value) : nullptr;
}

ICondition* CEqual::ResolveCGreater(const CGreater* cGreater) const
{
    return cGreater->ResolveCEqual(this);
}

ICondition* CEqual::ResolveCInSet(const CInSet* cInSet) const
{
    return cInSet->ResolveCEqual(this);
}
