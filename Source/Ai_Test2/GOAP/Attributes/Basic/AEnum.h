#pragma once
#include "IAttribute.h"



class AEnum : public IAttribute
{
public:
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        return (float)(value1 != value2); 
    }
    float GetMaxDifference() const override
    {
        return 1.0f;
    }
};