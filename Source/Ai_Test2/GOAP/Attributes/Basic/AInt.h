#pragma once
#include <algorithm>

#include "IAttribute.h"


class AInt : public IAttribute
{
public:
    AInt(t_value maxValue) : MAX_VALUE(maxValue) {}
    
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        return std::clamp((float)value1 - (float)value2, 0.0f, (float)MAX_VALUE);
    }
    float GetMaxDifference() const override
    {
        return (float)MAX_VALUE;
    }

    std::string GetEnumeratorString(t_value value) const override
    {
        return std::to_string(value);
    }
    
    float MakeMeanString(t_value newVal, int num, std::string& stringData) const
    {
        float newMean = newVal;
        if (stringData != "")
        {
            float prevMean = std::stof(stringData);
            newMean = (prevMean * num + (float)newVal) / (num + 1);
        }
        stringData = std::to_string(newMean);
        return newMean; 
    }
private:
    const t_value MAX_VALUE;
};