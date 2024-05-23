#pragma once
#include "../Basic/IAttribute.h"
#include "../../DataBase.h"

class AAtNode : public IAttribute
{
public:
    AAtNode()
    {
        _numPoints = DataPtr->Navigator->GetNumPoints();
    }
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
    void MakeStatString(t_value newVal, int num, std::string& stringData) const override
    {
        auto biasedValue = newVal + BIAS;
        MY_ASSERT(biasedValue >= 0 && biasedValue < _numPoints);
        std::string newSeriesString;
        if (stringData == "") //if its the first entry, we create series string
        {
            for (int i = 0; i < _numPoints; i++)
                if (i == biasedValue)
                    newSeriesString.append("1 ");
                else
                    newSeriesString.append("0 ");
            stringData = newSeriesString;
            return;
        }
        else
        {
            std::istringstream input(stringData);
            std::vector<int> frequencies(_numPoints);
            for (int i = 0; i < _numPoints; i++)
            {
                std::string frequencyString;
                std::getline(input, frequencyString, ' ');
                frequencies[i] = std::stoi(frequencyString);
                if (i == biasedValue)
                    frequencies[i]++;
                newSeriesString.append(std::to_string(frequencies[i]) + " ");
            }
            stringData = newSeriesString;
            return;
        }
    }
    inline static const float NAV_PATH_LENGTH_DIVISOR = 25.0f; //divide true distance to scale GoTo cost

private:
    int _numPoints;
    const t_value BIAS = 1; //value(index) = index - BIAS; index = value(index) + BIAS
};