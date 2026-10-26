#include "MissionAutoCycleController.h"
#include "MissionManager.h"
#include "DefenseMinigameController.h"
#include "Mission/BaseMission.h"
#include "Mission/DefenseMinigameMission.h"
#include "Character/NPC/NPC.h"
#include "Character/User/MissionSystemCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UMissionAutoCycleController::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UMissionManager* MissionManager = GetMissionManager();
	if(MissionManager != nullptr)
	{
		MissionManager->OnAnyMissionStateChanged.AddDynamic(this, &UMissionAutoCycleController::HandleMissionStateChanged);
	}
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

	// 데모 진행 속도를 위해 접근 중에는 일반 걷기 속도보다 빠르게 이동시킴.
	// Agree 처리 시 SetPlayMode(DefenseMode)가, 미션 종료 시 SetPlayMode(Normal)이
	// MaxWalkSpeed를 각 모드 값으로 다시 덮어쓰므로 별도로 복원할 필요는 없음
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

	// bForce=true: 미션 UI가 열려있는 동안(SetUICursorMode -> SetIgnoreMoveInput)에는 사람의 이동 입력을
	// 막는 게 맞지만, 자동 사이클은 그 UI 규칙과 무관하게 항상 이동할 수 있어야 함
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

	// 디펜스 모드는 월드 Y축(오른쪽)만 좌우 이동으로 취급함(AMissionSystemCharacter::Move() 참고).
	// 시간이 아니라 현재 방향으로 실제 이동한 거리가 DodgeSwitchDistance에 도달하면 방향 전환
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
				// 오버랩으로 미션창이 떴다는 뜻이므로 더 이상 이동할 필요 없음
				StopApproachingTargetNPC();
				World->GetTimerManager().SetTimer(ActionDelayTimerHandle, this, &UMissionAutoCycleController::TriggerAutoAgree, AutoAgreeDelay, false);
			}
			break;
		case EMissionState::InProgress:
			{
				// 디펜스 미니게임 시작 -> 좌우 회피 이동 시작
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

	// Start() 호출(OnInProgress) 전에 생존 시간을 미리 단축 설정해둠
	UDefenseMinigameMission* DefenseMission = Cast<UDefenseMinigameMission>(MissionManager->GetCurrentMission());
	if(DefenseMission != nullptr)
	{
		DefenseMission->GetOrCreateDefenseMinigameController()->SetSurvivalDurationOverride(AutoCycleSurvivalDuration);
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

	// 한 사이클(NPC 접근 -> Agree -> 디펜스 -> Confirm)만 돌고 자동으로 꺼짐
	bIsEnabled = false;
}

UMissionManager* UMissionAutoCycleController::GetMissionManager() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UMissionManager>();
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
