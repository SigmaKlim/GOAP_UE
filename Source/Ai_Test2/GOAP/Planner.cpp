#include "Planner.h"

#include "Condition/Basic/ConditionSet.h"
#include "Condition/Special/CEqual.h"


Plan::Plan(const Planner& planner, const ValueSet& startState, const std::string& goalName) :
                                StartState(startState), Goal(*planner._goalCatalogue.GetItem(goalName))
{
    GoalName = goalName;
    TotalCost = 0.0f;
}


template<>
size_t VertexKey<Vertex>::operator()(const Vertex& k) const
{
    size_t hash = 0;
    for (size_t i = 0; i < k.ActiveConditionSet.Size(); i++)
        if (k.ActiveConditionSet.IsAffected(i))
            MathHelper::CombineHash(hash, k.ActiveConditionSet.GetProperty(i));
    MathHelper::CombineHash(hash, k.PrevActionId);
    return hash;
}


Planner::Planner()
{
    ConditionSet::_attributeCataloguePtr = &this->_attributeCatalogue;
}


Plan Planner::ConstructPlan(const ValueSet& startState, std::string goalName, SupplementalData initData) const
{
    Plan plan(*this, startState, goalName);
    assert(startState.NumAffected() == startState.Size());
    //We convert startState to a condition set made of Equal conditions
    ConditionSet targetConditionSet(_attributeCatalogue.Size());
    for (unsigned i = 0; i < _attributeCatalogue.Size(); i++)
        targetConditionSet.SetCondition(i, new CEqual(plan.StartState.GetProperty(i)));
    Vertex targetVertex(targetConditionSet);
    
    Vertex departureVertex( plan.Goal,
                            std::numeric_limits<size_t>::max(),
                            0,
                            initData,
                            "");
    Path<Vertex> path;
    plan.success = Pathfind(path, departureVertex, targetVertex, &plan.TelemetryData);
    if (plan.success != false)
    {
        for (size_t i = path.Vertices.size() - 1; i > 0; i--) //iterate from back to start, because we build plan from the last action to the first
        {
            plan.ActionIds.push_back(path.Vertices[i].PrevActionId);
            plan.ActionNames.push_back(*_actionConstructorCatalogue.GetName(path.Vertices[i].PrevActionId));
            plan.ActionStrings.push_back(path.Vertices[i].PrevActionStringData);
        }
        plan.TotalCost = path.Cost;
    }
    return plan;
}

void Planner::GetNeighbors(std::vector<Vertex>& neighbors, std::vector<float>& distances, const Vertex& vertex, const Vertex& finish) const
{
    if (vertex.ActionCtr > MAX_NUM_ACTIONS_PER_PLAN)
        return;
    for (unsigned i = 0; i < _actionConstructorCatalogue.Size(); i++)
    {
        std::vector<Action> actions;
        (*_actionConstructorCatalogue.GetItem(i))->ConstructActions(actions, vertex.ActiveConditionSet, vertex.UserData);
        for (auto& action : actions)
        {
            ConditionSet reducedConditionSet(vertex.ActiveConditionSet.Size());
            bool isActionUseful = vertex.ActiveConditionSet.Reduce(action.Effects, reducedConditionSet, action.UserData);
            ConditionSet mergedConditionSet(vertex.ActiveConditionSet.Size());
            bool isActionLegit = reducedConditionSet.Merge(action.Conditions, mergedConditionSet);
            if (isActionUseful == true && isActionLegit == true)
            {
                neighbors.push_back({   mergedConditionSet, i,
                                        vertex.ActionCtr + 1,
                                        action.UserData,
                                        *_actionConstructorCatalogue.GetName(i),
                                        action.StringData});
                distances.push_back(action.Cost);
            }
        }
    }
}

bool Planner::Satisfies(const Vertex& vertex/*active cond set*/, const Vertex& finish/*start state*/) const
{
    return !vertex.ActiveConditionSet.HasConflicts(finish.ActiveConditionSet);
}

float Planner::GetDistanceDenominator() const
{
    static float accumActionCostSum = 0.0f;
    if (accumActionCostSum == 0.0f)
        for (auto& action : _actionConstructorCatalogue.ObjectIterator)
            accumActionCostSum += action->GetMaxCost();
    return accumActionCostSum;
}

float Planner::GetHeuristic(const Vertex& vertex/*active cond set*/, const Vertex& finish/*start state*/) const
{
    return 0.0f;
}

float Planner::GetHeuristicsDenominator() const
{
    return AStarSolver<Vertex>::GetHeuristicsDenominator();
}

size_t Planner::GetNumAttributes() const
{
    return _attributeCatalogue.Size();
}

size_t Planner::GetAttributeId(const std::string& name) const
{
    auto* idPtr = _attributeCatalogue.GetId(name);
    assert(idPtr);
    return *idPtr;
}



