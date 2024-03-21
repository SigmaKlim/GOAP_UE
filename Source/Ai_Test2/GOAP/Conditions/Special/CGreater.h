#pragma once
#include "../Basic/ICondition.h"
struct CGreater :  public ICondition
{
    CGreater(t_value value);
    float Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const override;

    //Condition resolution
    
    RESOLVE_D(CGreater)
    
    ICondition* ResolveCGreater(const CGreater* cGreater) const override;
    ICondition* ResolveCEqual(const CEqual* cEqual) const override;
    ICondition* ResolveCInSet(const CInSet* cInSet) const override;
    
    t_value Value;
};


