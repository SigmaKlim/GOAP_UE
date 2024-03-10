#pragma once
#include <cassert>
#include <string>
#include <unordered_map>
#include <unordered_set>


//A structure that holds ids of nodes, divided into groups by name, as well as distances between nodes (from, to -> dist)
struct Navigator
{
    //Adds nodes, but does not check for copies under other names
    void AddNode(const std::string& name, int id, std::vector<float> position);
    const std::unordered_set<int>& GetNodesByName(const std::string& name) const;
    //If either from or to is -1 (only 1 of them), pos is used instead 
    float GetDistance(int from, int to) const;
    float GetMaxDistance() const;
    
    bool IsSymmetric = true;
private:
    std::unordered_map<std::string, std::unordered_set<int>> nodeMap; //nodeName -> nodeIds
    std::unordered_map<int, std::vector<float>> positions;
    float maxDistance = 0;
};





