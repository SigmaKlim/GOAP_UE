// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapController.h"
#include "../SoftCheckMacro.h"
const Strategist* UGoapController::StrategistPtr;
const Planner* UGoapController::PlannerPtr;
const DataBase* UGoapController::DataPtr;




// Called when the game starts
void UGoapController::BeginPlay()
{
	Super::BeginPlay();
	Helper helper(*DataPtr);
	_agentStatePtr = std::make_shared<ValueSet>(helper.MakeValueSet({ {"AIsCrouching", false}, {"AIsPatrolling", false} }));
	_currentPlanPtr = std::make_shared<Plan>(DataPtr->GetNumAttributes());
    _goalPriorities.resize(DataPtr->GoalCatalogue.Size());
    for (size_t i = 0; i < _goalPriorities.size(); i++)
        _goalPriorities[i] = (*DataPtr->GoalCatalogue.GetItem(i))->UpdatePriority();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Goap Controller initialized!"));

}

void UGoapController::UpdateAi(bool wasActionComplete, bool mustBuildStrategy, FString& actionName, TMap<FString,int32>& effects)
{
    if (mustBuildStrategy == true)
    {
        StrategistPtr->ConstructStrategy(_goalPriorities, _currentStrategy);
        _currentGoalIndex = 0;
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Strategy rebuild called"));
    }
    if (wasActionComplete == true || mustBuildStrategy == true) //if we complete the action
    {
        if (mustBuildStrategy == true || ++_currentActionIndex >= _currentPlanPtr->ActionIds.size()) //if we complete the plan
        {
            (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->UpdatePriority();
            _currentGoalIndex = (_currentGoalIndex + 1) % DataPtr->GoalCatalogue.Size();

            _currentPlanPtr->Clear();
            _currentPlanPtr->Goal = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->GetConditions();
            _currentPlanPtr->GoalName = (*DataPtr->GoalCatalogue.GetName(_currentStrategy.GoalIds[_currentGoalIndex]));
            _currentPlanPtr->StartState = *_agentStatePtr;
            _isGoalFinished = false;
            MY_ASSERT(PlannerPtr->ConstructPlan(*_currentPlanPtr, GenerateSupData()));            
            _agentStatePtr = std::make_shared<ValueSet>
                ((*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->OverrideAgentState(_currentPlanPtr->ResultState));
            _currentActionIndex = 0;   
        }
        effects.Empty();
    }
    if (_currentPlanPtr->ActionIds.size() == 0)
        UpdateAi(true, false, actionName, effects);
    else
    {
        actionName = FString(DataPtr->ActionCatalogue.GetName(_currentPlanPtr->ActionIds[_currentActionIndex])->c_str());
        for (size_t i = 0; i < _currentPlanPtr->ActionInstances[_currentActionIndex].Effects.Size(); i++)
            if (_currentPlanPtr->ActionInstances[_currentActionIndex].Effects.IsAffected(i))
                effects.Add(FString(DataPtr->AttributeCatalogue.GetName(i)->c_str()),
                    _currentPlanPtr->ActionInstances[_currentActionIndex].Effects.GetValue(i));
    }
}



SupplementalData UGoapController::GenerateSupData()
{
    SupplementalData data;
    return data;
}


UGoapController::UGoapController()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UGoapController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   
}

