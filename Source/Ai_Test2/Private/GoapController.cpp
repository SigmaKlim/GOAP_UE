// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapController.h"
#include "GameFramework/Character.h"
#include "../SoftCheckMacro.h"
const Strategist* UGoapController::StrategistPtr;
const Planner* UGoapController::PlannerPtr;
const DataBase* UGoapController::DataPtr;

// Called when the game starts
void UGoapController::BeginPlay()
{
	Super::BeginPlay();
	Helper helper(*DataPtr);
    _currentState = helper.MakeValueSet({ {"AIsCrouching", false},
                                                {"AEnemyStatus", (int)EAVEnemyStatus::eNonVisible},
                                                {"AAmmoInMag", 20},
                                                {"AMagsLeft", 100},
                                                {"AAtNode", 0},
                                                {"AHpLeft", 100}});
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
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Strategy rebuild called"));
    }
    if (mustBuildStrategy == true || _currentActionIndex >= _currentPlan.ActionIds.size()) // <=> if we must build a new plan
    {
        _isStateKnown = false;

        _currentGoalIndex = (_currentGoalIndex + 1) % DataPtr->GoalCatalogue.Size();//move to the next goal
        _currentState = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->OnGoalTaken(_currentState);
        _currentPlan.Clear();
        _currentPlan.Goal = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->GetConditions();
        _currentPlan.GoalName = (*DataPtr->GoalCatalogue.GetName(_currentStrategy.GoalIds[_currentGoalIndex]));
        _currentPlan.StartState = _currentState;
        MY_ASSERT(PlannerPtr->ConstructPlan(_currentPlan, GenerateSupData()));
        SavePlanData(_currentPlan);
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

void UGoapController::GetCurrentPlan(TArray<int32>& actionIDs, TArray<float>& actionCosts) const
{
    actionIDs.Empty();
    actionCosts.Empty();
    for (int i = 0; i < _currentPlan.ActionIds.size(); i++)
    {
        actionIDs.Add(_currentPlan.ActionIds[i]);
        actionCosts.Add(_currentPlan.ActionInstances[i].Cost);
    }
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
    data.initNode = _currentState.GetValue(DataPtr->GetAttributeId("AAtNode"));
    data.futureGoToDestinationNode = -1;
    data.agentPtr = Cast<ACharacter>(GetOwner());
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

void UGoapController::SavePlanData(const Plan& plan) const
{
    const FString DATA_PATH = FPaths::ProjectUserDir() + "Data/";
    const FString KILL_ENEMY_PATH = DATA_PATH + "KillEnemy/";
    TArray<FString> newS0Strings; //start state to save
    for (int i = 0; i < plan.StartState.Size(); i++)
        newS0Strings.Add(FString::FromInt(plan.StartState.GetValue(i)));
    TArray<FString> savedS0Strings; //previously saved start states
    FFileHelper::LoadFileToStringArray(savedS0Strings, *(KILL_ENEMY_PATH + "s0.txt"));
    check(savedS0Strings.Num() % DataPtr->GetNumAttributes() == 0);

    std::vector<FString> savedS0Strings_v(savedS0Strings.GetData(), savedS0Strings.GetData() + savedS0Strings.Num());
    for (int i = 0; i < savedS0Strings_v.size(); i += DataPtr->GetNumAttributes())
    {
        if (std::equal(savedS0Strings_v.begin() + i,
            savedS0Strings_v.begin() + i + DataPtr->GetNumAttributes(), newS0Strings.GetData()))
        {
            _isStateKnown = true;
            _knownStateIndex = i / DataPtr->GetNumAttributes();
            break;
        }
    }
    if (_isStateKnown)
    {
        TArray<FString> ns;
        FFileHelper::LoadFileToStringArray(ns, *(KILL_ENEMY_PATH + "n.txt"));
        int n = FCString::Atoi(ns[_knownStateIndex].GetCharArray().GetData());
        int newN = n + 1;
        ns[_knownStateIndex] = FString::FromInt(newN);
        FFileHelper::SaveStringArrayToFile(ns, *(KILL_ENEMY_PATH + "n.txt"));
        TArray<FString> meanedSStrings;
        FFileHelper::LoadFileToStringArray(meanedSStrings, *(KILL_ENEMY_PATH + "s.txt"));
        std::vector<float> meanedState(DataPtr->GetNumAttributes());
        for (int i = 0; i < DataPtr->GetNumAttributes(); i++)
        {
            std::string cstring(TCHAR_TO_ANSI(*meanedSStrings[_knownStateIndex * DataPtr->GetNumAttributes() + i]));
            float meanedVal = (*DataPtr->AttributeCatalogue.GetItem(i))->MakeMeanString(
                plan.ResultState.GetValue(i), n, cstring);
            meanedSStrings[_knownStateIndex * DataPtr->GetNumAttributes() + i] = FString(cstring.c_str());
        }
        FFileHelper::SaveStringArrayToFile(meanedSStrings, *(KILL_ENEMY_PATH + "s.txt"));
    }
    else
    {
        FFileHelper::SaveStringArrayToFile(newS0Strings, *(KILL_ENEMY_PATH + "s0.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
        FFileHelper::SaveStringToFile(FString::FromInt(1) + "\n", *(KILL_ENEMY_PATH + "n.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
        TArray<FString> meanedSString; //planned state to save
        for (int i = 0; i < plan.StartState.Size(); i++)
        {
            std::string cstring = "";
            (*DataPtr->AttributeCatalogue.GetItem(i))->MakeMeanString(plan.ResultState.GetValue(i), 0, cstring);
                meanedSString.Add(FString(cstring.c_str()));
        }
        FFileHelper::SaveStringArrayToFile(meanedSString, *(KILL_ENEMY_PATH + "s.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
    }
}