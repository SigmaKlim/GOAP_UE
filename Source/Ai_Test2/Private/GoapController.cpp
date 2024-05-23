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
                                                {"AAmmoInMag", 30},
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
        if (mustBuildStrategy == false)
        {
            _currentState = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->OnGoalCompleted(_currentState);
            if (_mustSaveStatistics == true)
                SavePlanData(true); //report goal completion to statistics
        }
        _currentGoalIndex = (_currentGoalIndex + 1) % DataPtr->GoalCatalogue.Size();//move to the next goal
        _currentState = (*DataPtr->GoalCatalogue.GetItem(_currentStrategy.GoalIds[_currentGoalIndex]))->OnGoalTaken(_currentState);
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

void UGoapController::SavePlanData(bool isGoalCompleted) const
{
    //s0.txt - for start states
    //ns0.txt - for numbers each start state was discovered
    //s.txt - for result (planned) states
    //b.txt - stores bool for whether the goal was completed
    //ss.txt - for result (actual) states by the goal completion
    //nss.txt - for numbers each result state was achieved (not equal to nS0 as we ignore results when the goal was not completed)
    // 
    //todo: make hash table instead the array scanning
    const auto& plan = _currentPlan;
    const FString GOAL_STATISTICS_PATH = DATA_PATH + FString(plan.GoalName.c_str()) + "/";
    TArray<FString> newS0Strings; //start state to save
    for (int i = 0; i < plan.StartState.Size(); i++)
        newS0Strings.Add(FString::FromInt(plan.StartState.GetValue(i)));
    TArray<FString> savedS0Strings; //previously saved start states
    FFileHelper::LoadFileToStringArray(savedS0Strings, *(GOAL_STATISTICS_PATH + "s0.txt"));
    TArray<FString> savedNS0Strings;
    FFileHelper::LoadFileToStringArray(savedNS0Strings, *(GOAL_STATISTICS_PATH + "ns0.txt"));
    TArray<FString> meanedSStrings;
    FFileHelper::LoadFileToStringArray(meanedSStrings, *(GOAL_STATISTICS_PATH + "s.txt"));
    TArray<FString> savedBStrings; //goal completion status
    FFileHelper::LoadFileToStringArray(savedBStrings, *(GOAL_STATISTICS_PATH + "b.txt"));
    TArray<FString> meanedSSStrings; //actual states by the goal completion
    FFileHelper::LoadFileToStringArray(meanedSSStrings, *(GOAL_STATISTICS_PATH + "ss.txt"));
    TArray<FString> savedNSSStrings;
    FFileHelper::LoadFileToStringArray(savedNSSStrings, *(GOAL_STATISTICS_PATH + "nss.txt"));
    check(savedS0Strings.Num() / DataPtr->GetNumAttributes() == savedNS0Strings.Num());
    check(savedS0Strings.Num() == meanedSStrings.Num());
    check(savedS0Strings.Num() % DataPtr->GetNumAttributes() == 0);
    check(meanedSSStrings.Num() % DataPtr->GetNumAttributes() == 0);
    check(savedBStrings.Num() == meanedSSStrings.Num() / DataPtr->GetNumAttributes());
    check(savedBStrings.Num() == savedNSSStrings.Num());
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
    if (_isStateKnown == true)
    {
        int nS0 = FCString::Atoi(savedNS0Strings[_knownStateIndex].GetCharArray().GetData());
        savedNS0Strings[_knownStateIndex] = FString::FromInt(nS0 + 1);
        FFileHelper::SaveStringArrayToFile(savedNS0Strings, *(GOAL_STATISTICS_PATH + "ns0.txt"));
        std::vector<float> meanedState(DataPtr->GetNumAttributes());
        for (int i = 0; i < DataPtr->GetNumAttributes(); i++)
        {
            std::string cstring(TCHAR_TO_ANSI(*meanedSStrings[_knownStateIndex * DataPtr->GetNumAttributes() + i]));
            (*DataPtr->AttributeCatalogue.GetItem(i))->MakeStatString(plan.ResultState.GetValue(i), nS0, cstring);
            meanedSStrings[_knownStateIndex * DataPtr->GetNumAttributes() + i] = FString(cstring.c_str());
        }
        FFileHelper::SaveStringArrayToFile(meanedSStrings, *(GOAL_STATISTICS_PATH + "s.txt"));

        float b = FCString::Atof(savedBStrings[_knownStateIndex].GetCharArray().GetData());
        //we calculate mean b over all number of plans for given s0
        savedBStrings[_knownStateIndex] = FString::SanitizeFloat(MathHelper::UpdateMean(b, (float)isGoalCompleted, nS0));
        FFileHelper::SaveStringArrayToFile(savedBStrings, *(GOAL_STATISTICS_PATH + "b.txt"));
        if (isGoalCompleted == true) //is the goal is not completed, we do not update meaned attributes (is this correct?)
        {
            int nSS = FCString::Atoi(savedNSSStrings[_knownStateIndex].GetCharArray().GetData());
            savedNSSStrings[_knownStateIndex] = FString::FromInt(nSS + 1);
            FFileHelper::SaveStringArrayToFile(savedNSSStrings, *(GOAL_STATISTICS_PATH + "nss.txt"));
            for (int i = 0; i < DataPtr->GetNumAttributes(); i++)
            {
                std::string cstring(TCHAR_TO_ANSI(*meanedSSStrings[_knownStateIndex * DataPtr->GetNumAttributes() + i]));
                if (cstring == "@")
                    cstring = ""; //means this result state was not achieved before
                (*DataPtr->AttributeCatalogue.GetItem(i))->MakeStatString(_currentState.GetValue(i), nSS, cstring);
                meanedSSStrings[_knownStateIndex * DataPtr->GetNumAttributes() + i] = FString(cstring.c_str());
            }
            FFileHelper::SaveStringArrayToFile(meanedSSStrings, *(GOAL_STATISTICS_PATH + "ss.txt"));
        }
        
    }
    else
    {
        FFileHelper::SaveStringArrayToFile(newS0Strings, *(GOAL_STATISTICS_PATH + "s0.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
        FFileHelper::SaveStringToFile(FString::FromInt(1) + "\n", *(GOAL_STATISTICS_PATH + "ns0.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
        TArray<FString> meanedSString; //planned state to save
        for (int i = 0; i < plan.StartState.Size(); i++)
        {
            std::string cstring = "";
            (*DataPtr->AttributeCatalogue.GetItem(i))->MakeStatString(plan.ResultState.GetValue(i), 0, cstring);
                meanedSString.Add(FString(cstring.c_str()));
        }
        FFileHelper::SaveStringArrayToFile(meanedSString, *(GOAL_STATISTICS_PATH + "s.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

        FFileHelper::SaveStringToFile(FString::FromInt(isGoalCompleted) + "\n", *(GOAL_STATISTICS_PATH + "b.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
        FFileHelper::SaveStringToFile(FString::FromInt(isGoalCompleted) + "\n", *(GOAL_STATISTICS_PATH + "nss.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
        ValueSet stateToSave = (isGoalCompleted == true) ? _currentState : ValueSet(DataPtr->GetNumAttributes());
        TArray<FString> meanedSSString; //planned state to save
        for (int i = 0; i < plan.StartState.Size(); i++)
        {
            if (isGoalCompleted == true)
            {
                std::string cstring = "";
                (*DataPtr->AttributeCatalogue.GetItem(i))->MakeStatString(_currentState.GetValue(i), 0, cstring);
                meanedSSString.Add(FString(cstring.c_str()));
            }
            else
                meanedSSString.Add("@"); //placeholder for when the state wss not achieved to make indexing similar to s0.txt
        }
        FFileHelper::SaveStringArrayToFile(meanedSSString, *(GOAL_STATISTICS_PATH + "ss.txt"),
            FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
    }
}

void UGoapController::ReportGoalFailed() const
{
    if (_mustSaveStatistics == true)
        SavePlanData(false);
}


