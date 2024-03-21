// Fill out your copyright notice in the Description page of Project Settings.


#include "GoapController.h"

const Strategist* UGoapController::StrategistPtr;
const Planner* UGoapController::PlannerPtr;
const DataBase* UGoapController::DataPtr;

// Sets default values for this component's properties
UGoapController::UGoapController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoapController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGoapController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Goap is on!!!"));
	// ...
}

