#pragma once
#include "IAttribute.h"



class AEnum : public IAttribute
{
public:
    AEnum(const std::vector<std::string>& enumerators) : ENUMERATORS(enumerators) {}
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        return (float)(value1 != value2); 
    }
    float GetMaxDifference() const override
    {
        return 1.0f;
    }

    std::string GetEnumeratorString(t_value value) const override
    {
        MY_ASSERT(value < ENUMERATORS.size());
        return ENUMERATORS[value];
    }
private:
    std::vector<std::string> ENUMERATORS;
};