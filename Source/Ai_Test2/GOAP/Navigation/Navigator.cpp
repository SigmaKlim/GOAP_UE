#include "Navigator.h"

#include <cmath>



void Navigator::AddNode(const std::string& name, int id, std::vector<float> position)
{
    auto fres = nodeMap.find(name);
    if (fres == nodeMap.end())
        nodeMap.insert({name, {id}});
    else
        assert(fres->second.insert(id).second);
    assert(position.size() == 3);
    assert(positions.insert({id, position}).second);
}

const std::unordered_set<int>& Navigator::GetNodesByName(const std::string& name) const
{
    auto fres = nodeMap.find(name);
    assert(fres != nodeMap.end());
    return fres->second;
}

float Navigator::GetDistance(int from, int to) const
{
    assert(from != -1 && to != -1);
    auto fresFrom = positions.find(from);
    auto& fromPos = fresFrom->second;
    assert(fresFrom != positions.end());
    auto fresTo = positions.find(to);
    assert(fresTo != positions.end());
    auto& toPos = fresTo->second;
    return sqrt(powf(toPos[0] - fromPos[0],2.0f) +
                powf(toPos[1] - fromPos[1],2.0f) +
                powf(toPos[2] - fromPos[2],2.0f));
}

float Navigator::GetMaxDistance() const
{
    return maxDistance;
}