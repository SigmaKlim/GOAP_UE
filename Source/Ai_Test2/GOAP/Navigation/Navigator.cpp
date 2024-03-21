#include "Navigator.h"

#include <cmath>



void Navigator::AddNode(const std::string& name, int id, std::vector<float> position)
{
    auto fres = _nodeMap.find(name);
    if (fres == _nodeMap.end())
        _nodeMap.insert({name, {id}});
    else
        assert(fres->second.insert(id).second);
    assert(position.size() == 3);
    assert(_positions.insert({id, position}).second);
}

const std::unordered_set<int>& Navigator::GetNodesByName(const std::string& name) const
{
    auto fres = _nodeMap.find(name);
    assert(fres != _nodeMap.end());
    return fres->second;
}

float Navigator::GetDistance(int from, int to) const
{
    assert(from != -1 && to != -1);
    if (from == 0 || to == 0)
        return (float)rand() / RAND_MAX * _maxDistance;
    auto fresFrom = _positions.find(from);
    auto& fromPos = fresFrom->second;
    assert(fresFrom != _positions.end());
    auto fresTo = _positions.find(to);
    assert(fresTo != _positions.end());
    auto& toPos = fresTo->second;
    return sqrt(powf(toPos[0] - fromPos[0],2.0f) +
                powf(toPos[1] - fromPos[1],2.0f) +
                powf(toPos[2] - fromPos[2],2.0f));
}

float Navigator::GetMaxDistance() const
{
    return _maxDistance;
}

std::string Navigator::GetNodeName(int nodeId) const
{
    if (nodeId == -1)
        return "UNKNOWN";
    for (auto& nodes : _nodeMap)
        if (nodes.second.contains(nodeId))
            return nodes.first;
    assert(false);
    return "";
}
