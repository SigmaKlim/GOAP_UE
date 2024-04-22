#pragma once
#include "IAttribute.h"
#include <algorithm>
#include <iostream>
#include "../../DataBase.h"

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
    float MakeMeanString(t_value newVal, int num, std::string& stringData) const
    {
        std::string newSeriesString;
        if (stringData == "") //if its the first entry, we create series string
        {
            for (int i = 0; i < DataPtr->GetNumAttributes(); i++)
                if (i == newVal)
                    newSeriesString.append("1 ");
                else
                    newSeriesString.append("0 ");
            stringData = newSeriesString;
            return newVal;
        }
        else
        {
            std::istringstream input(stringData);
            std::vector<int> frequencies(DataPtr->GetNumAttributes());
            for (int i = 0; i < DataPtr->GetNumAttributes(); i++)
            {
                std::string frequencyString;
                std::getline(input, frequencyString, ' ');
                frequencies[i] = std::stoi(frequencyString);
                if (i == newVal)
                    frequencies[i]++;
                newSeriesString.append(std::to_string(frequencies[i]) + " ");
            }
            stringData = newSeriesString;
            return std::distance(frequencies.begin(),
                std::max_element(frequencies.begin(), frequencies.end()));
        }
    }
private:
    std::vector<std::string> ENUMERATORS;
};