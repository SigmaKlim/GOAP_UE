#pragma once
#include <unordered_set>

#include "../Basic/ICondition.h"
//assume that we use InSet only for enum-like attributes, so the distance between two values is 1 or 0
struct CInSet : public ICondition
{
    CInSet(const std::unordered_set<t_value>& valueSet);
    float Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const override;

    //Condition resolution

    RESOLVE_D(CInSet)
    ICondition* ResolveCEqual(const CEqual* cEqual) const override;
    ICondition* ResolveCGreater(const CGreater* cGreater) const override;
    ICondition* ResolveCInSet(const CInSet* cInSet) const override;
    
    std::unordered_set<t_value> Values;
};


