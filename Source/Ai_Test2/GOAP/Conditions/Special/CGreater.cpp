#include "CGreater.h"
#include "CEqual.h"
#include "CInSet.h"

CGreater::CGreater(t_value value): Value(value){}

float CGreater::Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const
{
    return value > Value ? 0.0f : attributePtr->GetDifference(value, Value, userData);
}

ICondition* CGreater::ResolveCGreater(const CGreater* cGreater) const
{
    const t_value& a = Value;
    const t_value& b = cGreater->Value;
    if (a > b)
        return new CGreater(a);
    return new CGreater(b);
}

ICondition* CGreater::ResolveCEqual(const CEqual* cEqual) const
{
    if (cEqual->Value > Value)
        return new CEqual(Value);
    return nullptr;
}

ICondition* CGreater::ResolveCInSet(const CInSet* cInSet) const
{
    return cInSet->ResolveCGreater(this);
}