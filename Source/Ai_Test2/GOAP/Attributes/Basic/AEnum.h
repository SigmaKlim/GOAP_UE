#pragma once
#include "IAttribute.h"
#include <algorithm>
#include <iostream>
#include "../../DataBase.h"

class AEnum : public IAttribute
{
public:
    AEnum(const std::vector<std::string>& enumerators) : _enumerators(enumerators), _numEnumerators(enumerators.size()) {}
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        MY_ASSERT(value1 < _numEnumerators && value2 < _numEnumerators);
        return (float)(value1 != value2); 
    }
    float GetMaxDifference() const override
    {
        return 1.0f;
    }
    std::string GetEnumeratorString(t_value value) const override
    {
        MY_ASSERT(value < _numEnumerators);
        return _enumerators[value];
    }
    void MakeStatString(t_value newVal, int num, std::string& stringData) const override
    {
        MY_ASSERT(newVal < _numEnumerators);
        std::string newSeriesString;
        if (stringData == "") //if its the first entry, we create series string
        {
            bool foundVal = false;
            for (int i = 0; i < _numEnumerators; i++)
                if (i == newVal)
                {
                    newSeriesString.append("1 ");
                    foundVal = true;
                }
                else
                    newSeriesString.append("0 ");
            stringData = newSeriesString;
            MY_ASSERT(foundVal == true);
            return;
        }
        else
        {
            std::istringstream input(stringData);
            std::vector<int> frequencies(_numEnumerators);
            bool foundVal = false;
            for (int i = 0; i < _numEnumerators; i++)
            {
                std::string frequencyString;
                std::getline(input, frequencyString, ' ');
                frequencies[i] = std::stoi(frequencyString);
                if (i == newVal)
                {
                    frequencies[i]++;
                    foundVal = true;
                }
                newSeriesString.append(std::to_string(frequencies[i]) + " ");
            }
            MY_ASSERT(foundVal == true);
            stringData = newSeriesString;
            return;
        }
    }
private:
    std::vector<std::string> _enumerators;
    int _numEnumerators;
};