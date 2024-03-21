#pragma once
#include "../Basic/IAttribute.h"

class AAtNode : public IAttribute
{
public:
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        if (value1 == -1 || value2 == -1) //we reserve node == -1 for the case when the real value is unknown 
            return DataPtr->Navigator.GetMaxDistance() / 2; 
        return DataPtr->Navigator.GetDistance(value1, value2);
    }
    float GetMaxDifference() const override
    {
        return DataPtr->Navigator.GetMaxDistance();
    }
    std::string GetEnumeratorString(t_value value) const override
    {
        return DataPtr->Navigator.GetNodeName(value) + " (" + std::to_string(value) + ")"; 
    }
    
};