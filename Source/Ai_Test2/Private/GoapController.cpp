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
	_agentStatePtr = std::make_shared<ValueSet>(helper.MakeValueSet({ {"AIsCrouching", false} }));
	_currentPlanPtr = std::make_shared<Plan>(DataPtr->GetNumAttributes());
    _goalPriorities.resize(DataPtr->GoalCatalogue.Size());
    for (size_t i = 0; i < _goalPriorities.size(); i++)
        _goalPriorities[i] = (*DataPtr->GoalCatalogue.GetItem(i))->UpdatePriority();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Goap Controller initialized!"));

}

int UGoapController::UpdateAi(bool wasActionComplete, bool mustBuildStrategy)
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
            //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Goal completed!"));
        }
    }
    return _currentPlanPtr->ActionIds.size() > 0 ? _currentPlanPtr->ActionIds[_currentActionIndex] 
        : UpdateAi(true, false);
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

