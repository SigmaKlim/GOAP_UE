#pragma once
#include "../Basic/ICondition.h"
struct CLarger :  public ICondition
{
    CLarger(t_value value);
    float Evaluate(t_value value, const IAttribute* attributePtr, const SupplementalData& userData) const override;

    //Condition resolution
    
    RESOLVE_D(CLarger)
    
    ICondition* ResolveCLarger(const CLarger* cLarger) const override;
    ICondition* ResolveCEqual(const CEqual* cEqual) const override;
    ICondition* ResolveCInSet(const CInSet* cInSet) const override;
    
    t_value Value;
};


