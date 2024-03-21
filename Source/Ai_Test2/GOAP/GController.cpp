#include "GController.h"
#include <numeric>

const Strategist*    GController::StrategistPtr;
const Planner*       GController::PlannerPtr;
const DataBase*      GController::DataPtr;

GController::GController(const ValueSet& initState, const std::vector<std::shared_ptr<IActionPerformer>>& performers) : _agentState(initState), _currentPlan(DataPtr->GetNumAttributes()), _actionPerformers(performers)
{
    _goalPriorities.resize(DataPtr->GoalCatalogue.Size());
    for (size_t i = 0; i < _goalPriorities.size(); i++)
        _goalPriorities[i] = (*DataPtr->GoalCatalogue.GetItem(i))->UpdatePriority();
}

void GController::Update()
{
    if (_mustBuildStrategy == true)
    {
        StrategistPtr->ConstructStrategy(_goalPriorities, _currentStrategy);
        _currentGoalIndex = 0;
        _mustBuildStrategy = false;
        _isGoalFinished = true;
        //debug
        std::cout << "Strategy rebuild called!.\n";
        //
    }
    if(_isGoalFinished == true)
    {
        //debug
        static bool skipNextDebugPrint = true;
        if (skipNextDebugPrint == false)
            std::cout << "Goal " + _currentPlan.GoalName + " completed!\n";
        else
            skipNextDebugPrint = false;
        //
        _currentPlan.Clear();
        _currentPlan.Goal = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->GetConditions();
        _currentPlan.GoalName = (*DataPtr->GoalCatalogue.GetName(_currentStrategy.GoalIds[_currentGoalIndex]));
        _currentPlan.StartState = _agentState;
        _isGoalFinished = false;
        MY_ASSERT(PlannerPtr->ConstructPlan(_currentPlan, GenerateSupData()));
        _currentActionIndex = 0;
        //debug
        std::cout << "Plan for goal " << _currentPlan.GoalName << " constructed!\n";
        _debugger.PrintPlan(_currentPlan);
        //
    }
    if (_currentPlan.ActionIds.size() == 0 || _actionPerformers[_currentPlan.ActionIds[_currentActionIndex]]->PerformAction() == true) //if we complete the action
    {
        _currentActionIndex++; //move on to the next action in the plan
        if (_currentActionIndex >= _currentPlan.ActionIds.size()) //if we complete the plan
        {
            (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->UpdatePriority();
            _currentGoalIndex = (_currentGoalIndex + 1) % DataPtr->GoalCatalogue.Size();
            _agentState = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->OverrideAgentState(_currentPlan.ResultState);
            _isGoalFinished = true;
            //debug
            std::cout << "\nResult state:\n";
            _debugger.PrintValueSet(_agentState);
            std::cout << "\n";
            //
        }
    }
}

void GController::UpdateGoalPriority(const std::string& name, bool mustRebuildStrategy)
{
    auto goalPtr = *DataPtr->GoalCatalogue.GetItem(name);
    auto goalId = *DataPtr->GoalCatalogue.GetId(name);
    MY_ASSERT(goalPtr != nullptr);
    _goalPriorities[goalId] = goalPtr->UpdatePriority();
    _mustBuildStrategy = mustRebuildStrategy;
}

void GController::RequestStrategyRebuild()
{
    _mustBuildStrategy = true;
}


SupplementalData GController::GenerateSupData() const
{
    SupplementalData data;
    data.initNode = _agentState.GetValue(DataPtr->GetAttributeId("atNode"));
    data.futureGoToDestinationNode = -1;
    return data;
}


