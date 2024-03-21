// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
class Helper;

UCLASS()
class AI_TEST2_API AMyGameMode : public AGameModeBase
{
	AMyGameMode();
	GENERATED_BODY()
	virtual void BeginPlay() override;

	void InitializeGoap();

	void InitializeGAttributes(const Helper& helper);
	void InitializeGActions(const Helper& helper);
};
