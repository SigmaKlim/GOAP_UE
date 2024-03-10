#pragma once


#include "AStar.h"
#include "Tools/Catalogue.h"
#include "Actions/Basic/IActionConstructor.h"



struct Plan
{
    Plan(const class Planner& planner, const ValueSet& startState, const std::string& goalName);
    ValueSet StartState;
    std::string GoalName;
    
    ConditionSet Goal;

    bool success = false;
    std::vector<size_t> ActionIds;
    std::vector<std::string> ActionNames;
    std::vector<std::string> ActionStrings;
    
    float TotalCost;
    TelemetryData TelemetryData;
};

struct Vertex
{
    ConditionSet ActiveConditionSet; //A set of conditions required for all previously taken actions
    std::size_t PrevActionId;
    unsigned ActionCtr; //counter of previously included actions
    
    SupplementalData UserData;
    //debug
    std::string PrevActionName;
    std::string PrevActionStringData;
};

template<>
size_t VertexKey<Vertex>::operator()(const Vertex& k) const;


class Planner : public AStarSolver<Vertex>
{
public:
    Planner();
    
    void GetNeighbors(std::vector<Vertex>& neighbors, std::vector<float>& distances, const Vertex& vertex, const Vertex& finish) const override;
    bool Satisfies(const Vertex& vertex, const Vertex& finish) const override;
    float GetDistanceDenominator() const override;
    float GetHeuristic(const Vertex& vertex, const Vertex& finish) const override;
    float GetHeuristicsDenominator() const override;

    template <typename t_attribute>
    void RegisterAttribute(const std::string& name);
    template <typename t_action>
    void RegisterActionConstructor(const std::string& name, const t_action& actionConstructor);
    void RegisterGoal(const std::string& name, const ConditionSet& goalState);

    //For convenient use
    Plan ConstructPlan(const ValueSet& startState, std::string goalName, SupplementalData initData) const;
    size_t GetNumAttributes() const;
    size_t GetAttributeId(const std::string& name) const;
private:
    Catalogue<IAttribute*> _attributeCatalogue;
    Catalogue<IActionConstructor*> _actionConstructorCatalogue;
    Catalogue<ConditionSet> _goalCatalogue;

    const unsigned MAX_NUM_ACTIONS_PER_PLAN = 10;
    friend Plan;
    friend class Helper;
};

template <typename t_attribute>
void Planner::RegisterAttribute(const std::string& name)
{
    IAttribute* iAttributePtr = static_cast<IAttribute*>(new t_attribute);
    assert(_attributeCatalogue.AddItem(name, iAttributePtr));
}

template <typename t_action>
void Planner::RegisterActionConstructor(const std::string& name, const t_action& actionConstructor)
{
    auto* iActionConstructor = static_cast<IActionConstructor*>(new t_action(actionConstructor));
    assert(_actionConstructorCatalogue.AddItem(name, iActionConstructor));
}

inline void Planner::RegisterGoal(const std::string& name, const ConditionSet& goalState)
{
    assert(_goalCatalogue.AddItem(name, goalState));    
}