// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../GOAP/GInclude.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	int UpdateAi(bool wasActionComplete, bool mustBuildStrategy);
	

	static const Strategist* StrategistPtr;
	static const Planner* PlannerPtr;
	static const DataBase* DataPtr;


private:
	SupplementalData GenerateSupData();

    std::shared_ptr<ValueSet> _agentStatePtr;
    int _currentActionIndex = 0;
    std::shared_ptr<Plan> _currentPlanPtr;
    int _currentGoalIndex = -1;
    Strategy _currentStrategy;
    std::vector<float> _goalPriorities;

    bool _mustBuildStrategy = true;
    bool _isGoalFinished = false;
    bool _mustBuildPlan = true;

	friend DataBase;
	friend Strategist;
	friend Planner;
};
