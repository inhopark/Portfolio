#include "MissionManager.h"
#include "Factory/MissionFactory.h"
#include "UMG/MainMissionWidget.h"
#include "UMG/MissionResultWidget.h"
#include "Character/User/MissionSystemCharacter.h"
#include "Character/NPC/NPC.h"
#include "Controllers/MainPlayerController.h"

void UMissionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	MissionFactory = NewObject<UMissionFactory>(this);
	MissionFactory->Initialize();
}

void UMissionManager::RegisterMainMissionWidget(UMainMissionWidget* Widget)
{
	MainMissionWidget = Widget;

	if(MainMissionWidget != nullptr)
	{
		MainMissionWidget->OnMissionButtonAction.AddDynamic(this, &UMissionManager::HandleMissionButtonAction);
	}
}

void UMissionManager::RegisterMissionResultWidget(UMissionResultWidget* ResultWidget)
{
	MissionResultWidget = ResultWidget;

	if(ResultWidget != nullptr)
	{
		ResultWidget->OnConfirmed.AddDynamic(this, &UMissionManager::HandleMissionResultConfirmed);
	}
}

void UMissionManager::ShowMainMissionWidget(EMissionUnique MissionUnique, ANPC* InNPC)
{
	if(MainMissionWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionManager ## ShowMainMissionWidget ## MainMissionWidget Not Registered ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
		return;
	}

	if(CurrentMission != nullptr)
	{
		return;
	}

	CurrentMissionNPC = InNPC;

	SetUICursorMode(true);
	MainMissionWidget->Show();
	RequestMission(MissionUnique);
}

void UMissionManager::HideMainMissionWidget()
{
	SetUICursorMode(false);

	if(MainMissionWidget != nullptr)
	{
		MainMissionWidget->Hide();
	}
}

void UMissionManager::HandleMissionButtonAction(EMissionButtonAction Action)
{
	if(CurrentMission == nullptr)
	{
		return;
	}

	if(CurrentMission->GetMissionState() != EMissionState::Ready)
	{
		return;
	}

	switch (Action)
	{
		case EMissionButtonAction::Left:
			{
				if(CurrentMissionNPC != nullptr)
				{
					CurrentMissionNPC->SetActorHiddenInGame(true);
					CurrentMissionNPC->SetActorEnableCollision(false);
				}

				AMissionSystemCharacter* PlayerCharacter = GetPlayerCharacter();
				if(PlayerCharacter != nullptr)
				{
					PlayerCharacter->SetPlayMode(ECharacterPlayMode::DefenseMode);
				}

				CurrentMission->AgreeMission(PlayerCharacter);

				SetDefenseHUDVisible(true);

				HideMainMissionWidget();
			}
			break;
		case EMissionButtonAction::Right:
			{
				CurrentMission->DisagreeMission();

				ClearCurrentMission();
			}
			break;
	}
}

void UMissionManager::HandlePlayerDefeated()
{
	if(CurrentMission != nullptr)
	{
		CurrentMission->FailedMission();
	}
}

void UMissionManager::HandleMissionResultConfirmed()
{
	if(MissionResultWidget != nullptr)
	{
		MissionResultWidget->Hide();
	}

	ClearCurrentMission();
}

float UMissionManager::GetMissionRemainingTime() const
{
	if(CurrentMission == nullptr)
	{
		return -1.0f;
	}

	return CurrentMission->GetMinigameRemainingTime();
}

float UMissionManager::GetMissionTotalDuration() const
{
	if(CurrentMission == nullptr)
	{
		return 0.0f;
	}

	return CurrentMission->GetMinigameTotalDuration();
}

void UMissionManager::RequestMission(EMissionUnique MissionUnique)
{
	if(IsMissionRequest() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("## MissionManager ## RequestMission ## Warning : Mission Already In Progress ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
		return;
	}

	if(MissionFactory != nullptr)
	{
		CurrentMission = MissionFactory->CreateMission(this, MissionUnique);
		if(CurrentMission != nullptr)
		{
			CurrentMission->OnMissionStateChanged.AddDynamic(this, &UMissionManager::HandleMissionStateChanged);

			CurrentMission->Initialize();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionManager ## RequestMission ## Mission Factory Not Initialized ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
	}
}

bool UMissionManager::IsMissionRequest()
{
	return CurrentMission == nullptr;
}

void UMissionManager::HandleMissionStateChanged(EMissionState NewState)
{
	switch (NewState)
	{
		case EMissionState::Ready:
		case EMissionState::InProgress:
			{
				if(MainMissionWidget != nullptr)
				{
					MainMissionWidget->SetWidgetState(NewState);
				}
			}
			break;
		case EMissionState::Succeeded:
		case EMissionState::Failed:
			{
				SetDefenseHUDVisible(false);

				ShowMissionResultWidget(NewState);
			}
			break;
		default:
			{
			}
			break;
	}

	OnAnyMissionStateChanged.Broadcast(NewState);
}

void UMissionManager::SetUICursorMode(bool bShow)
{
	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(PlayerController == nullptr)
	{
		return;
	}

	PlayerController->bShowMouseCursor = bShow;
	PlayerController->SetIgnoreLookInput(bShow);

	PlayerController->SetIgnoreMoveInput(bShow);

	UE_LOG(LogTemp, Log, TEXT("## MissionManager ## SetUICursorMode ## bShow : %s"), *LexToString(bShow));
}

void UMissionManager::ShowMissionResultWidget(EMissionState NewState)
{
	if(MissionResultWidget == nullptr)
	{
		return;
	}

	SetUICursorMode(true);

	MissionResultWidget->SetResultState(NewState);
	MissionResultWidget->Show();
}

void UMissionManager::ClearCurrentMission()
{
	if(CurrentMission != nullptr)
	{
		CurrentMission->OnMissionStateChanged.RemoveDynamic(this, &UMissionManager::HandleMissionStateChanged);

		CurrentMission = nullptr;
	}

	AMissionSystemCharacter* PlayerCharacter = GetPlayerCharacter();
	if(PlayerCharacter != nullptr)
	{
		PlayerCharacter->ResetPlayer();
	}

	if(CurrentMissionNPC != nullptr)
	{
		CurrentMissionNPC->SetActorHiddenInGame(false);
		CurrentMissionNPC->SetActorEnableCollision(true);
		CurrentMissionNPC = nullptr;
	}

	HideMainMissionWidget();
}

void UMissionManager::SetDefenseHUDVisible(bool bVisible)
{
	AMainPlayerController* MainPlayerController = GetMainPlayerController();
	if(MainPlayerController != nullptr)
	{
		MainPlayerController->SetDefenseHUDVisible(bVisible);
	}
}

AMissionSystemCharacter* UMissionManager::GetPlayerCharacter() const
{
	AMainPlayerController* MainPlayerController = GetMainPlayerController();
	if(MainPlayerController == nullptr)
	{
		return nullptr;
	}

	return Cast<AMissionSystemCharacter>(MainPlayerController->GetPawn());
}

AMainPlayerController* UMissionManager::GetMainPlayerController() const
{
	const UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return nullptr;
	}

	return Cast<AMainPlayerController>(World->GetFirstPlayerController());
}
