// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../GOAP/GInclude.h"
#include <chrono>
#include <ActionPerformer.h>
#include "MyGameMode.generated.h"


UCLASS()
class AI_TEST2_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameMode();
	virtual void BeginPlay() override;

	void InitializeGoap();
	void InitializeGAttributes(DataBase & data);
	void InitializeGActions(DataBase& data, const Helper& helper);
	void InitializeGGoals(DataBase& data, const Helper& helper);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSecondsSinceStart() const;

	UFUNCTION(BlueprintCallable)
	bool SetActionPerformer(const FString& actionName, UActionPerformer* performer);

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> AttributeNames;
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> ActionNames;
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> GoalNames;

private:
	std::shared_ptr<DataBase> GlobalDataPtr;
	std::shared_ptr<Strategist> GlobalStrategistPtr;
	std::shared_ptr<Planner> GlobalPlannerPtr;

	const std::chrono::system_clock::time_point START_TIME = std::chrono::system_clock::now();
	
};
