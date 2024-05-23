// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterestPoint.h"
#include <unordered_set>
#include <vector>
#include "NavigatorN.generated.h"

UCLASS()
class AI_TEST2_API ANavigatorN : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavigatorN();

	std::string GetNodeName(int id) const;
	const AInterestPoint* GetNodeObject(int id) const;
	const std::unordered_set<int>& GetNodesByTag(EInterestTag tag);
	float GetDistance(int from, int to, const ACharacter* agent) const;
	UFUNCTION(BlueprintImplementableEvent)
	float GetDistance(const AActor* from, const AActor* to) const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetMaxDistance() const;

	int GetNumPoints() const;
	int GetNumTags() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	TMap<int, AInterestPoint*> InterestPoints;
private:
	std::vector<std::unordered_set<int>> organizer;
};
