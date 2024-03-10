#pragma once
#include "../Basic/IAttribute.h"
#include "../../Navigation/Navigator.h"

class AAtNode : public IAttribute
{
public:
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        if (value1 == -1 || value2 == -1) //we reserve node == -1 for the case when the real value is unknown 
            return navigator.GetMaxDistance() / 2; 
        return navigator.GetDistance(value1, value2);
    }
    float GetMaxDifference() const override
    {
        return navigator.GetMaxDistance();
    }

    static Navigator navigator;
};