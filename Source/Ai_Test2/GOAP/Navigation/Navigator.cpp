#include "Navigator.h"
#include "../Tools/AssertMacro.h"
#include <cmath>



Navigator_old::Navigator_old()
{
}

void Navigator_old::AddNode(const std::string& name, int id, std::vector<float> position)
{
    //auto fres = _nodeMap.find(name);
    //if (fres == _nodeMap.end())
    //    _nodeMap.insert({name, {id}});
    //else
    //    MY_ASSERT(fres->second.insert(id).second);
    //MY_ASSERT(position.size() == 3);
    //MY_ASSERT(_positions.insert({id, position}).second);
}

const std::unordered_set<int>& Navigator_old::GetNodesByName(const std::string& name) const
{
    auto fres = _nodeMap.find(name);
    MY_ASSERT(fres != _nodeMap.end());
    return fres->second;
}

float Navigator_old::GetDistance(int from, int to) const
{
    MY_ASSERT(from != -1 && to != -1);
    if (from == 0 || to == 0)
        return (float)rand() / RAND_MAX * _maxDistance;
    auto fresFrom = _positions.find(from);
    auto& fromPos = fresFrom->second;
    MY_ASSERT(fresFrom != _positions.end());
    auto fresTo = _positions.find(to);
    MY_ASSERT(fresTo != _positions.end());
    auto& toPos = fresTo->second;
    return sqrt(powf(toPos[0] - fromPos[0],2.0f) +
                powf(toPos[1] - fromPos[1],2.0f) +
                powf(toPos[2] - fromPos[2],2.0f));
}

float Navigator_old::GetMaxDistance() const
{
    return _maxDistance;
}

std::string Navigator_old::GetNodeName(int nodeId) const
{
    if (nodeId == -1)
        return "UNKNOWN";
    for (auto& nodes : _nodeMap)
        if (nodes.second.contains(nodeId))
            return nodes.first;
    MY_ASSERT(false);
    return "";
}
