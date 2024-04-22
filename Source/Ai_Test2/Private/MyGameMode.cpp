// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "../GOAP/GInclude.h"
#include <Misc/AssertionMacros.h>
#include "../SoftCheckMacro.h"
#include "Kismet/GameplayStatics.h"
#include "NavigatorN.h"
#include "InterestPoint.h"
#include "EngineUtils.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ConstructorHelpers::FClassFinder<APawn>(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")).Class;
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitializeGoap();
	GetWorld()->bIsCameraMoveableWhenPaused = true;

}


void AMyGameMode::InitializeGoap()
{
	GlobalDataPtr = std::make_shared<DataBase>();
	TArray<AActor*> navigators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavigatorN::StaticClass(), navigators);
	FString s = FString::FromInt(navigators.Num()) + " navigators found.";
	GlobalDataPtr->Navigator = Cast<ANavigatorN>(navigators[0]);
	Helper helper(*GlobalDataPtr);
	InitializeGAttributes(*GlobalDataPtr);
	InitializeGActions(*GlobalDataPtr, helper);
	InitializeGGoals(*GlobalDataPtr, helper);
	GlobalStrategistPtr = std::make_shared<Strategist>(*GlobalDataPtr);
	GlobalPlannerPtr = std::make_shared<Planner>(*GlobalDataPtr);
}



void AMyGameMode::InitializeGAttributes(DataBase & data)
{
	SOFT_CHECK(data.RegisterAttribute("AIsCrouching", new ABool), "Failed to register AIsCrouching.");
	SOFT_CHECK(data.RegisterAttribute("AEnemyStatus", new AEnemyStatus), "Failed to register AEnemyStatus");
	SOFT_CHECK(data.RegisterAttribute("AAmmoInMag", new AAmmoInMag), "Failed to register AAmmoInMag");
	SOFT_CHECK(data.RegisterAttribute("AMagsLeft", new AMagsLeft), "Failed to register AMagsLeft");
	SOFT_CHECK(data.RegisterAttribute("AAtNode", new AAtNode), "Failed to register AAtNode");
	SOFT_CHECK(data.RegisterAttribute("AHpLeft", new AHealth), "Failed to register AHpLeft")

	for (auto& aName : data.AttributeCatalogue.nRange)
		AttributeNames.Add(FString(aName->c_str()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Attributes have been registered."));
}

void AMyGameMode::InitializeGActions(DataBase& data, const Helper& helper)
{
	ConditionSet	cCrouch = helper.MakeConditionSet({});
	ValueSet		eCrouch = helper.MakeValueSet({ {"AIsCrouching", true} });
	SOFT_CHECK(data.RegisterAction("AcCrouch", new AcSimple(cCrouch, eCrouch, 1)), "Failed to register AcCrouch.");

	ConditionSet	cStand = helper.MakeConditionSet({});
	ValueSet		eStand = helper.MakeValueSet({ {"AIsCrouching", false} });
	SOFT_CHECK(data.RegisterAction("AcStand", new AcSimple(cStand, eStand, 1)), "Failed to register AcStand.");

	SOFT_CHECK(data.RegisterAction("AcPatrol", new AcPatrol(	data.GetAttributeId("AEnemyStatus"),
																			data.GetAttributeId("AIsCrouching"),
																			data.GetAttributeId("AAtNode"), 
																			1)), "Failed to register AcPatrol.");

	ConditionSet	cApproach = helper.MakeConditionSet({ {"AEnemyStatus", new CEqual((int)EAVEnemyStatus::eVisible) } });
	ValueSet		eApproach = helper.MakeValueSet({	{"AEnemyStatus", (int)EAVEnemyStatus::eInCombatRadius},
															{"AAtNode", -1}});
	SOFT_CHECK(data.RegisterAction("AcApproach", new AcSimple(cApproach, eApproach, 1)), "Failed to register AcApproach.");

	SOFT_CHECK(data.RegisterAction("AcShoot", new AcShoot(	data.GetAttributeId("AEnemyStatus"), 
																		data.GetAttributeId("AAmmoInMag"), 5)), "Failed to register AcShoot.");

	SOFT_CHECK(data.RegisterAction("AcReload", new ACUseDepletable(data.GetAttributeId("AMagsLeft"),
																				data.GetAttributeId("AAmmoInMag"), 20, 15, "magazines", "ammo")),
																						"Failed to register AcReload.");
	SOFT_CHECK(data.RegisterAction("AcReloadFromCover", new ACReloadFromCover(	data.GetAttributeId("AMagsLeft"),
																							data.GetAttributeId("AAmmoInMag"), 
																							data.GetAttributeId("AAtNode"), 
																							data.GetAttributeId("AIsCrouching"), 
																							20, 1)),
		"Failed to register AcReloadFromCover");
	SOFT_CHECK(	data.RegisterAction("AcGoTo", new AcGoTo(data.GetAttributeId("AAtNode"),
				data.GetAttributeId("AEnemyStatus"),
				data.GetAttributeId("AIsCrouching"))), "Failed to register AcGoTo.");

	//SOFT_CHECK(data.RegisterAction("AcHeal", new ))

	for (auto& aName : data.ActionCatalogue.nRange)
		ActionNames.Add(FString(aName->c_str()));
	data.ActionPerformers.resize(data.ActionCatalogue.Size());
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Actions have been registered."));

}

void AMyGameMode::InitializeGGoals(DataBase& data, const Helper& helper)
{
	/*ConditionSet gCrouch = helper.MakeConditionSet({ {"AIsCrouching", new CEqual(EAVBool::eTrue)} });
	SOFT_CHECK(data.RegisterGoal("GCrouch", new GTest(gCrouch, 4.0f)), "Failed to register GCrouch.");*/

	/*ConditionSet gStand = helper.MakeConditionSet({ {"AIsCrouching", new CEqual(EAVBool::eFalse)} });
	SOFT_CHECK(data.RegisterGoal("GStand", new GTest(gStand, 5.0f)), "Failed to register GStand.");*/

	/*ConditionSet gPatrol = helper.MakeConditionSet({ {"AIsPatrolling", new CEqual(EAVBool::eTrue)} });
	SOFT_CHECK(data.RegisterGoal("GPatrol", new GTest(gPatrol, 5.0f)), "Failed to register GPatrol.");*/

	ConditionSet gHide = helper.MakeConditionSet({		{"AAtNode", new CInSet(data.Navigator->GetNodesByTag(EInterestTag::eCover))},
															{"AIsCrouching", new CEqual(true)}});
	//SOFT_CHECK(data.RegisterGoal("GHide", new GTest(gHide, 5.0f)), "Failed to register GHide");

	SOFT_CHECK(data.RegisterGoal("GKillEnemy", new GKill), "Failed to register GKillEnemy.");

	for (auto& gName : data.GoalCatalogue.nRange)
		GoalNames.Add(FString(gName->c_str()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Goals have been registered."));
}

float AMyGameMode::GetSecondsSinceStart() const
{
	return GetWorld()->GetTimeSeconds();
}

bool AMyGameMode::SetActionPerformer(const FString& actionName, UActionPerformer* performer)
{
	std::string cName = TCHAR_TO_UTF8(*actionName);
	if (GlobalDataPtr->ActionCatalogue.GetId(cName) == nullptr)
	{
		std::string debugMes = "SetActionPerformer: Failed to find action under name " + cName;
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString(debugMes.c_str()));
		return false;
	}
	size_t id = *GlobalDataPtr->ActionCatalogue.GetId(cName);
	GlobalDataPtr->ActionPerformers[id] = performer;
	return false;
}