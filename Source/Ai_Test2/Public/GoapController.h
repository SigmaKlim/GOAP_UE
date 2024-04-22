// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GOAP/GInclude.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Containers/UnrealString.h>
#include <Containers/Map.h>

#include "GoapController.generated.h"



UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AI_TEST2_API UGoapController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoapController();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAi(bool wasActionComplete, bool mustBuildStrategy, FString& actionName, TMap<FString,int32>& effects);
	
	UFUNCTION(BlueprintCallable)
	void GetCurrentStrategy(TArray<int32>& goalIDs) const;

	UFUNCTION(BlueprintCallable)
	void GetCurrentPlan(TArray<int32>& actionIDs, TArray<float>& actionCosts) const;

	UFUNCTION(BlueprintCallable)
	void GetCurrentState(TArray<int32>& attributeValues) const;

	//Returns the index of current goal in current strategy
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentGoalIndex() const;

	//Returns the index of current action in current plan
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentActionIndex() const;

	UFUNCTION(BlueprintCallable)
	FString GetAttributeValueString(int32 attributeId, int32 attributeValue) const;

	UFUNCTION(BlueprintCallable)
	void UpdateGoalPriority(const FString& goalName, float priority);

	//Changes agent current state.
	UFUNCTION(BlueprintCallable)
	void OverrideAgentState(const TMap<FString, int32>& attributeValueList);

	//Returns true if the event completes current action, otherwise returns false (i.e. strategy must be rebuild!)
	UFUNCTION(BlueprintCallable)
	bool IsSurprised(const TMap<FString,int32>& effects) const;

	void SavePlanData(const Plan& plan) const;

	static const Strategist* StrategistPtr;
	static const Planner* PlannerPtr;
	static const DataBase* DataPtr;


private:
	SupplementalData GenerateSupData();

	ValueSet _expectedState; //We expect agent to arrive to this state by the completion of current action
    ValueSet _currentState;	 //State agent current state
    int _currentActionIndex = 0;
    Plan _currentPlan;
    int _currentGoalIndex = -1;
    Strategy _currentStrategy;
    std::vector<float> _goalPriorities;

	//for statistics
	mutable bool _isStateKnown = false;
	mutable int _knownStateIndex;

	friend DataBase;
	friend Strategist;
	friend Planner;
};
