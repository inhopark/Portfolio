#include "MissionAutoCycleController.h"
#include "Manager/MissionManager.h"
#include "DefenseMinigameController.h"
#include "Mission/BaseMission.h"
#include "Mission/DefenseMinigameMission.h"
#include "Character/NPC/NPC.h"
#include "Character/User/MissionSystemCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UMissionAutoCycleController::Initialize()
{
	UMissionManager* MissionManager = GetMissionManager();
	if(MissionManager == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionAutoCycleController ## Initialize ## Error : MissionManager Not Found"));
		return;
	}

	MissionManager->OnAnyMissionStateChanged.AddUniqueDynamic(this, &UMissionAutoCycleController::HandleMissionStateChanged);
}

void UMissionAutoCycleController::Deinitialize()
{
	StopApproachingTargetNPC();
	StopDodging();

	UWorld* World = GetWorld();
	if(World != nullptr)
	{
		World->GetTimerManager().ClearTimer(ActionDelayTimerHandle);
	}

	UMissionManager* MissionManager = GetMissionManager();
	if(MissionManager != nullptr)
	{
		MissionManager->OnAnyMissionStateChanged.RemoveDynamic(this, &UMissionAutoCycleController::HandleMissionStateChanged);
	}

	TargetNPC = nullptr;
	bIsEnabled = false;
}

UWorld* UMissionAutoCycleController::GetWorld() const
{
	if(HasAnyFlags(RF_ClassDefaultObject) == true)
	{
		return nullptr;
	}

	UObject* Outer = GetOuter();
	if(Outer == nullptr)
	{
		return nullptr;
	}

	return Outer->GetWorld();
}

void UMissionAutoCycleController::Tick(float DeltaTime)
{
	if(bIsApproachingNPC == true)
	{
		AdvanceTowardTargetNPC();
	}

	if(bIsDodging == true)
	{
		AdvanceDodgeMovement();
	}
}

void UMissionAutoCycleController::ToggleAutoCycleMode()
{
	bIsEnabled = bIsEnabled == false;

	UE_LOG(LogTemp, Log, TEXT("## MissionAutoCycleController ## ToggleAutoCycleMode ## AutoCycleMode : %s"), bIsEnabled == true ? TEXT("ON") : TEXT("OFF"));

	if(bIsEnabled == true)
	{
		StartApproachingTargetNPC();
	}
	else
	{
		StopApproachingTargetNPC();
		StopDodging();

		UWorld* World = GetWorld();
		if(World != nullptr)
		{
			World->GetTimerManager().ClearTimer(ActionDelayTimerHandle);
		}
	}
}

void UMissionAutoCycleController::StartApproachingTargetNPC()
{
	AMissionSystemCharacter* PlayerCharacter = GetPlayerCharacter();
	if(PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionAutoCycleController ## StartApproachingTargetNPC ## Player Character Not Found"));
		bIsEnabled = false;
		return;
	}

	TargetNPC = FindNearestNPC(PlayerCharacter);
	if(TargetNPC == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionAutoCycleController ## StartApproachingTargetNPC ## NPC Not Found In Level"));
		bIsEnabled = false;
		return;
	}

	UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
	if(MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = AutoApproachMoveSpeed;
	}

	bIsApproachingNPC = true;
}

void UMissionAutoCycleController::StopApproachingTargetNPC()
{
	bIsApproachingNPC = false;
}

void UMissionAutoCycleController::AdvanceTowardTargetNPC()
{
	AMissionSystemCharacter* PlayerCharacter = GetPlayerCharacter();
	if(PlayerCharacter == nullptr || TargetNPC == nullptr)
	{
		StopApproachingTargetNPC();
		return;
	}

	const FVector Direction = (TargetNPC->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal2D();

	PlayerCharacter->AddMovementInput(Direction, 1.0f, true);
}

void UMissionAutoCycleController::StartDodging()
{
	bIsDodging = true;
	bIsDodgeMovingRight = true;

	AMissionSystemCharacter* PlayerCharacter = GetPlayerCharacter();
	if(PlayerCharacter != nullptr)
	{
		DodgeLegStartLocation = PlayerCharacter->GetActorLocation();
	}
}

void UMissionAutoCycleController::StopDodging()
{
	bIsDodging = false;
}

void UMissionAutoCycleController::AdvanceDodgeMovement()
{
	AMissionSystemCharacter* PlayerCharacter = GetPlayerCharacter();
	if(PlayerCharacter == nullptr)
	{
		StopDodging();
		return;
	}

	const float TraveledDistance = FMath::Abs(PlayerCharacter->GetActorLocation().Y - DodgeLegStartLocation.Y);
	if(TraveledDistance >= DodgeSwitchDistance)
	{
		bIsDodgeMovingRight = bIsDodgeMovingRight == false;
		DodgeLegStartLocation = PlayerCharacter->GetActorLocation();
	}

	const FVector RightDirection(0.0f, 1.0f, 0.0f);
	const float Scale = bIsDodgeMovingRight == true ? 1.0f : -1.0f;

	PlayerCharacter->AddMovementInput(RightDirection, Scale, true);
}

void UMissionAutoCycleController::HandleMissionStateChanged(EMissionState NewState)
{
	if(bIsEnabled == false)
	{
		return;
	}

	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return;
	}

	switch (NewState)
	{
		case EMissionState::Ready:
			{
				StopApproachingTargetNPC();
				World->GetTimerManager().SetTimer(ActionDelayTimerHandle, this, &UMissionAutoCycleController::TriggerAutoAgree, AutoAgreeDelay, false);
			}
			break;
		case EMissionState::InProgress:
			{
				StartDodging();
			}
			break;
		case EMissionState::Succeeded:
		case EMissionState::Failed:
			{
				StopDodging();
				World->GetTimerManager().SetTimer(ActionDelayTimerHandle, this, &UMissionAutoCycleController::TriggerAutoConfirm, AutoConfirmDelay, false);
			}
			break;
		default:
			{
			}
			break;
	}
}

void UMissionAutoCycleController::TriggerAutoAgree()
{
	UMissionManager* MissionManager = GetMissionManager();
	if(MissionManager == nullptr)
	{
		return;
	}

	UDefenseMinigameMission* DefenseMission = Cast<UDefenseMinigameMission>(MissionManager->GetCurrentMission());
	if(DefenseMission != nullptr && DefenseMission->GetDefenseMinigameController() != nullptr)
	{
		DefenseMission->GetDefenseMinigameController()->SetSurvivalDurationOverride(AutoCycleSurvivalDuration);
	}

	MissionManager->HandleMissionButtonAction(EMissionButtonAction::Left);
}

void UMissionAutoCycleController::TriggerAutoConfirm()
{
	UMissionManager* MissionManager = GetMissionManager();
	if(MissionManager != nullptr)
	{
		MissionManager->HandleMissionResultConfirmed();
	}

	TargetNPC = nullptr;

	bIsEnabled = false;

	UE_LOG(LogTemp, Log, TEXT("## MissionAutoCycleController ## TriggerAutoConfirm ## AutoCycleMode : OFF"));
}

UMissionManager* UMissionAutoCycleController::GetMissionManager() const
{
	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return nullptr;
	}

	return UGameInstance::GetSubsystem<UMissionManager>(World->GetGameInstance());
}

AMissionSystemCharacter* UMissionAutoCycleController::GetPlayerCharacter() const
{
	return Cast<AMissionSystemCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

ANPC* UMissionAutoCycleController::FindNearestNPC(const AMissionSystemCharacter* PlayerCharacter) const
{
	if(PlayerCharacter == nullptr)
	{
		return nullptr;
	}

	TArray<AActor*> FoundNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), FoundNPCs);

	ANPC* NearestNPC = nullptr;
	float NearestDistanceSquared = 0.0f;

	for(AActor* Actor : FoundNPCs)
	{
		ANPC* NPC = Cast<ANPC>(Actor);
		if(NPC == nullptr)
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(PlayerCharacter->GetActorLocation(), NPC->GetActorLocation());
		if(NearestNPC == nullptr || DistanceSquared < NearestDistanceSquared)
		{
			NearestDistanceSquared = DistanceSquared;
			NearestNPC = NPC;
		}
	}

	return NearestNPC;
}
