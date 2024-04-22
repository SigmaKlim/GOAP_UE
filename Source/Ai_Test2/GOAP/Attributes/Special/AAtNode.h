#pragma once
#include "../Basic/IAttribute.h"
#include "../../DataBase.h"

class AAtNode : public IAttribute
{
public:
    float GetDifference(t_value value1, t_value value2, const SupplementalData& userData) const override
    {
        if (value1 == -1 || value2 == -1) //we reserve node == -1 for the case when the real value is unknown 
            return DataPtr->Navigator->GetMaxDistance() / 2 / AAtNode::NAV_PATH_LENGTH_DIVISOR;
        return DataPtr->Navigator->GetDistance(value1, value2, userData.agentPtr) / AAtNode::NAV_PATH_LENGTH_DIVISOR;
    }
    float GetMaxDifference() const override
    {
        return DataPtr->Navigator->GetMaxDistance() / AAtNode::NAV_PATH_LENGTH_DIVISOR;
    }
    std::string GetEnumeratorString(t_value value) const override
    {
        return DataPtr->Navigator->GetNodeName(value) + " (" + std::to_string(value) + ")"; 
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
    inline static const float NAV_PATH_LENGTH_DIVISOR = 25.0f; //divide true distance to scale GoTo cost
};