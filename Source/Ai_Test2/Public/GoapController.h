// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GOAP/Strategist.h"
#include "../GOAP/Planner.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoapController.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_TEST2_API UGoapController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	static const Strategist* StrategistPtr;
	static const Planner* PlannerPtr;
	static const DataBase* DataPtr;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
