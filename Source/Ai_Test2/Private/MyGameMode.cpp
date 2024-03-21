// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "../GOAP/Planner.h"
#include "../GOAP/Strategist.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ConstructorHelpers::FClassFinder<APawn>(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")).Class;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	DataBase globalData;
	Planner globalPlanner(globalData);
	Strategist globalStrategist(globalData);
	
}
