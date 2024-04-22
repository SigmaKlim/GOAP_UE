// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigatorN.h"
#include "EngineUtils.h"
#include "InterestPoint.h"
#include "../GOAP/Tools/AssertMacro.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ANavigatorN::ANavigatorN()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	organizer.resize((unsigned)EInterestTag::eCount);
}

std::string ANavigatorN::GetNodeName(int id) const
{
	if (id == -1)
		return "Unknown";
	if (id == 0)
		return "Start";
	return TCHAR_TO_UTF8(*UEnum::GetValueAsString((*InterestPoints.Find(id))->tag));
}

const AInterestPoint* ANavigatorN::GetNodeObject(int id) const
{
	return *InterestPoints.Find(id);
}

const std::unordered_set<int>& ANavigatorN::GetNodesByTag(EInterestTag tag)
{
	return organizer[(unsigned)tag];
}

float ANavigatorN::GetDistance(int from, int to, const ACharacter* agent) const
{
	MY_ASSERT(from != -1 && to != -1);
	if (from == 0)
		return (GetDistance(Cast<AActor>(agent), *InterestPoints.Find(to)));
	if (to == 0)
		return (GetDistance(*InterestPoints.Find(from), Cast<AActor>(agent)));
	return GetDistance(*InterestPoints.Find(from), *InterestPoints.Find(to));

}

// Called when the game starts or when spawned
void ANavigatorN::BeginPlay()
{
	Super::BeginPlay();
	InterestPoints.Add(TTuple<int, AInterestPoint*>(0, nullptr)); //placeholder for element 0
	int ctr = 1;
	for (TActorIterator<AInterestPoint> it(GetWorld()); it; ++it)
	{
		organizer[(unsigned)it->tag].insert(ctr);
		InterestPoints.Add(TTuple<int, AInterestPoint*>(ctr, *it));
		ctr++;
	}
	FString s = FString::FromInt(InterestPoints.Num() - 1) + " interest points registered.";
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, s);
}

// Called every frame
void ANavigatorN::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

