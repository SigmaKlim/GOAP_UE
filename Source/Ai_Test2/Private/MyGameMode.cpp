// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "../GOAP/GInclude.h"
#include <Misc/AssertionMacros.h>
#include "../SoftCheckMacro.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ConstructorHelpers::FClassFinder<APawn>(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")).Class;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	

}

void AMyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	InitializeGoap();
}

void AMyGameMode::InitializeGoap()
{
	GlobalDataPtr = std::make_shared<DataBase>();
	Helper helper(*GlobalDataPtr);
	InitializeGAttributes(*GlobalDataPtr);
	InitializeGActions(*GlobalDataPtr, helper);
	InitializeGGoals(*GlobalDataPtr, helper);
	GlobalStrategistPtr = std::make_shared<Strategist>(*GlobalDataPtr);
	GlobalPlannerPtr = std::make_shared<Planner>(*GlobalDataPtr);
}


void AMyGameMode::InitializeGAttributes(DataBase & data)
{
	SOFT_CHECK(data.RegisterAttribute("AIsCrouching", new ABool), "Failed to register AIsCrouching.");
	SOFT_CHECK(data.RegisterAttribute("AIsPatrolling", new ABool), "Failed to register AIsPatrolling.");

	for (auto& aName : data.AttributeCatalogue.nRange)
		AttributeNames.Add(FString(aName->c_str()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Attributes have been registered."));
}

void AMyGameMode::InitializeGActions(DataBase& data, const Helper& helper)
{
	ConditionSet	cCrouch = helper.MakeConditionSet({});
	ValueSet		eCrouch = helper.MakeValueSet({ {"AIsCrouching", true} });
	SOFT_CHECK(data.RegisterAction("AcCrouch", new AcSimple(cCrouch, eCrouch, 3)), "Failed to register AcCrouch.");

	ConditionSet	cStand = helper.MakeConditionSet({});
	ValueSet		eStand = helper.MakeValueSet({ {"AIsCrouching", false} });
	SOFT_CHECK(data.RegisterAction("AcStand", new AcSimple(cStand, eStand, 4)), "Failed to register AcStand.");

	ConditionSet	cPatrol = helper.MakeConditionSet({});
	ValueSet		ePatrol = helper.MakeValueSet({ {"AIsPatrolling", true} });
	SOFT_CHECK(data.RegisterAction("AcPatrol", new AcSimple(cPatrol, ePatrol, 4)), "Failed to register AcPatrol.");

	for (auto& aName : data.ActionCatalogue.nRange)
		ActionNames.Add(FString(aName->c_str()));
	data.ActionPerformers.resize(data.ActionCatalogue.Size());
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Actions have been registered."));

}

void AMyGameMode::InitializeGGoals(DataBase& data, const Helper& helper)
{
	/*ConditionSet gCrouch = helper.MakeConditionSet({ {"AIsCrouching", new CEqual(EAVBool::eTrue)} });
	SOFT_CHECK(data.RegisterGoal("GCrouch", new GTest(gCrouch, 5.0f)), "Failed to register GCrouch.");

	ConditionSet gStand = helper.MakeConditionSet({ {"AIsCrouching", new CEqual(EAVBool::eFalse)} });
	SOFT_CHECK(data.RegisterGoal("GStand", new GTest(gStand, 5.0f)), "Failed to register GStand.");*/

	ConditionSet gPatrol = helper.MakeConditionSet({ {"AIsPatrolling", new CEqual(EAVBool::eTrue)} });
	SOFT_CHECK(data.RegisterGoal("GPatrol", new GTest(gPatrol, 5.0f)), "Failed to register GPatrol.");

	for (auto& gName : data.GoalCatalogue.nRange)
		GoalNames.Add(FString(gName->c_str()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Goals have been registered."));
}

float AMyGameMode::GetSecondsSinceStart() const
{
	return GetWorld()->GetTimeSeconds();
}

bool AMyGameMode::SetActionPerformer(const FString& actionName, UActionPerformer* performer)
{
	std::string cName = TCHAR_TO_UTF8(*actionName);
	if (GlobalDataPtr->ActionCatalogue.GetId(cName) == nullptr)
	{
		std::string debugMes = "SetActionPerformer: Failed to find action under name " + cName;
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString(debugMes.c_str()));
		return false;
	}
	size_t id = *GlobalDataPtr->ActionCatalogue.GetId(cName);
	GlobalDataPtr->ActionPerformers[id] = performer;
	return false;
}