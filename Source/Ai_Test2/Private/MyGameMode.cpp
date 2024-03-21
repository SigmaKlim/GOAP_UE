// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "../GOAP/GInclude.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ConstructorHelpers::FClassFinder<APawn>(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")).Class;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitializeGoap();
}

void AMyGameMode::InitializeGoap()
{
	DataBase globalData;
	Helper helper(globalData);
	InitializeGAttributes(helper);

	Planner globalPlanner(globalData);
	Strategist globalStrategist(globalData);
}


void AMyGameMode::InitializeGAttributes(const Helper& helper)
{
	helper.RegisterAttribute("AIsCrouching", new ABool);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Attributes have been registered."));
}

void AMyGameMode::InitializeGActions(const Helper& helper)
{
	ConditionSet	cCrouch = helper.MakeConditionSet({});
	ValueSet		eCrouch = helper.MakeValueSet({ {"AIsCrouching", true} });
	helper.RegisterAction("AcCrouch", new AcSimple(cCrouch, eCrouch, 3));

	ConditionSet	cStand = helper.MakeConditionSet({});
	ValueSet		eStand = helper.MakeValueSet({ {"AIsCrouching", false} });
	helper.RegisterAction("AcStand", new AcSimple(cStand, eStand, 4));
}
