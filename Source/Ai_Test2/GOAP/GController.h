#pragma once
#include <memory>

#include "Tools/Catalogue.h"
#include "Attributes/Basic/IAttribute.h"
#include "Goals/Basic/Goal.h"
#include "Planner.h"
#include "Actions/Performers/IActionPerformer.h"
#include "Debugger.h"
#include "Strategist.h"

struct RebuildStatus
{
    bool mustRebuild = false;
    bool mustDropCurrentGoal = false;
};


class GController
{
public:
    GController(const ValueSet& initState, const std::vector<std::shared_ptr<IActionPerformer>>& performers);
    void Update();
    void UpdateGoalPriority(const std::string& name, bool mustRebuildStrategy);
    void RequestStrategyRebuild();

    SupplementalData GenerateSupData() const;
    //Set value in the agent state. Can be called in BeginPlay or on goal completion
    
    static const Strategist*    StrategistPtr;
    static const Planner*       PlannerPtr;
    static const DataBase*      DataPtr;
private:
    ValueSet _agentState;
    int _currentActionIndex = -1;
    Plan _currentPlan;
    std::vector<std::shared_ptr<IActionPerformer>> _actionPerformers;
    int _currentGoalIndex = -1;
    Strategy _currentStrategy;
    std::vector<float> _goalPriorities;
    
    bool _mustBuildStrategy = true;
    bool _isGoalFinished = false;
    bool _mustBuildPlan = true;

    Debugger _debugger;
    
    friend Debugger;
    friend class Helper;
};
