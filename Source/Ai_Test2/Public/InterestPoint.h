// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterestPoint.generated.h"
UENUM(BlueprintType)
enum class EInterestTag : uint8
{
	eCover	UMETA(DisplayName = "Cover"),
	eHealthStation UMETA(DisplayName = "HealthStation"),
	eCount	UMETA(Hidden)

};

UCLASS()
class AI_TEST2_API AInterestPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInterestPoint();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EInterestTag tag = EInterestTag::eCover;

	UPROPERTY(BlueprintReadOnly)
	int id;

	UFUNCTION(BlueprintNativeEvent)
	bool IsReallyInteresting(const ACharacter* agentPtr) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsVisibleToPlayer() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
