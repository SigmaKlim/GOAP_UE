// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Containers/Map.h>
#include "ActionPerformer.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AI_TEST2_API UActionPerformer : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool Perform(const TMap<FString, int32>& effects);
};
