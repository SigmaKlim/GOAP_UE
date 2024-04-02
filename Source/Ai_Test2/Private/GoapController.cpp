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
	_currentState = helper.MakeValueSet({ {"AIsCrouching", false}, {"AEnemyStatus", (int)EAVEnemyStatus::eNonVisible} });
    _expectedState = ValueSet(DataPtr->GetNumAttributes());
	_currentPlan = Plan(DataPtr->GetNumAttributes());
    _goalPriorities.resize(DataPtr->GoalCatalogue.Size());
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Goap Controller initialized!"));

}

void UGoapController::UpdateAi(bool wasActionComplete, bool mustBuildStrategy, FString& actionName, TMap<FString,int32>& effects)
{
    bool mustUpdateOutput = false;
    if (mustBuildStrategy == true)
    {
        StrategistPtr->ConstructStrategy(_goalPriorities, _currentStrategy);
        _currentGoalIndex = -1;
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Strategy rebuild called"));
    }
    if (mustBuildStrategy == true || _currentActionIndex >= _currentPlan.ActionIds.size()) // <=> if we must build a new plan
    {
        if (mustBuildStrategy == false) //if the previous goal was completed
            _currentState = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->OverrideAgentState(_currentState);
        _currentGoalIndex = (_currentGoalIndex + 1) % DataPtr->GoalCatalogue.Size();//move to the next goal
        _currentPlan.Clear();
        _currentPlan.Goal = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->GetConditions();
        _currentPlan.GoalName = (*DataPtr->GoalCatalogue.GetName(_currentStrategy.GoalIds[_currentGoalIndex]));
        _currentPlan.StartState = _currentState;
        MY_ASSERT(PlannerPtr->ConstructPlan(_currentPlan, GenerateSupData()));
        if (_currentPlan.ActionIds.size() == 0) //if current state satisfies goal 
        {
            _currentActionIndex++; //move on to next goal
            UpdateAi(false, false, actionName, effects);
        }
        _currentActionIndex = 0;
        _expectedState = _currentState;
        _expectedState.Modify(_currentPlan.ActionInstances[_currentActionIndex].Effects);
        mustUpdateOutput = true;
    }
    else if (wasActionComplete == true)
    {
        _currentState = _expectedState;
        if (++_currentActionIndex < _currentPlan.ActionIds.size())
            _expectedState.Modify(_currentPlan.ActionInstances[_currentActionIndex].Effects);
        else
            UpdateAi(false, false, actionName, effects);
        mustUpdateOutput = true;
    }
    if (mustUpdateOutput == true)
    {
        effects.Empty();
        actionName = FString(DataPtr->ActionCatalogue.GetName(_currentPlan.ActionIds[_currentActionIndex])->c_str());
        for (size_t i = 0; i < _currentPlan.ActionInstances[_currentActionIndex].Effects.Size(); i++)
            if (_currentPlan.ActionInstances[_currentActionIndex].Effects.IsAffected(i))
                effects.Add(FString(DataPtr->AttributeCatalogue.GetName(i)->c_str()),
                    _currentPlan.ActionInstances[_currentActionIndex].Effects.GetValue(i));
    }
}

void UGoapController::GetCurrentStrategy(TArray<int32>& goalIDs) const
{
    goalIDs.Empty();
    for (auto& goalID : _currentStrategy.GoalIds)
        goalIDs.Add(goalID);
}

void UGoapController::GetCurrentPlan(TArray<int32>& actionIDs) const
{
    actionIDs.Empty();
    for (auto& actionID : _currentPlan.ActionIds)
        actionIDs.Add(actionID);
}

void UGoapController::GetCurrentState( TArray<int32>& attributeValues) const
{
    attributeValues.Empty(_currentState.Size());
    for (size_t i = 0; i < _currentState.Size(); i++)
        attributeValues.Add(_currentState.GetValue(i));

}

int32 UGoapController::GetCurrentGoalIndex() const
{
    return _currentGoalIndex;
}

int32 UGoapController::GetCurrentActionIndex() const
{
    return _currentActionIndex;
}

FString UGoapController::GetAttributeValueString(int32 attributeId, int32 attributeValue) const
{
    return FString((*DataPtr->AttributeCatalogue.GetItem(attributeId))->GetEnumeratorString(attributeValue).c_str());
}

void UGoapController::UpdateGoalPriority(const FString& goalName, float priority)
{
    std::string s(TCHAR_TO_UTF8(*goalName));
    _goalPriorities[*DataPtr->GoalCatalogue.GetId(s)] = priority;
}

void UGoapController::OverrideAgentState(const TMap<FString, int32>& attributeValueList)
{
    for (auto& pair : attributeValueList)
    {
        std::string attributeName(TCHAR_TO_UTF8(*pair.Key));
        _currentState.SetValue(DataPtr->GetAttributeId(attributeName), pair.Value);
    }
}

bool UGoapController::IsSurprised(const TMap<FString,int32>& effects) const
{
    auto affectedState = _currentState;
    for (auto& pair : effects)
    {
        std::string attributeName(TCHAR_TO_UTF8(*pair.Key));
        affectedState.SetValue(DataPtr->GetAttributeId(attributeName), pair.Value);
    }
    return _expectedState != affectedState;
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

