#include "MainPlayerController.h"
#include "Manager/MissionManager.h"
#include "Manager/UIManager.h"
#include "Controllers/MissionAutoCycleController.h"
#include "UMG/MainMissionWidget.h"
#include "UMG/MissionResultWidget.h"
#include "UMG/HPWidget.h"
#include "UMG/MissionTimerWidget.h"
#include "Engine/LocalPlayer.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UUIManager* UIManager = GetUIManager();
	if(UIManager == nullptr)
	{
		return;
	}

	UIManager->InitializeUI(this);

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>();
	if(MissionManager != nullptr)
	{
		MissionManager->RegisterMainMissionWidget(UIManager->GetUI<UMainMissionWidget>(EUIType::MainMission));
		MissionManager->RegisterMissionResultWidgets(UIManager->GetUI<UMissionResultWidget>(EUIType::MissionResult));
	}
}

void AMainPlayerController::SetDefenseHUDVisible(bool bVisible)
{
	UUIManager* UIManager = GetUIManager();
	if(UIManager == nullptr)
	{
		return;
	}

	UHPWidget* HPWidget = UIManager->GetUI<UHPWidget>(EUIType::HP);
	UMissionTimerWidget* MissionTimerWidget = UIManager->GetUI<UMissionTimerWidget>(EUIType::MissionTimer);
	if(HPWidget == nullptr || MissionTimerWidget == nullptr)
	{
		return;
	}

	if(bVisible == true)
	{
		HPWidget->Show();
		MissionTimerWidget->Show();
	}
	else
	{
		HPWidget->Hide();
		MissionTimerWidget->Hide();
	}

	UE_LOG(LogTemp, Log, TEXT("## MainPlayerController ## SetDefenseHUDVisible ## bVisible : %s"), *LexToString(bVisible));
}

void AMainPlayerController::ToggleAutoCycleMode()
{
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionAutoCycleController* AutoCycleController = GameInstance->GetSubsystem<UMissionAutoCycleController>();
	if(AutoCycleController != nullptr)
	{
		AutoCycleController->ToggleAutoCycleMode();
	}
}

UUIManager* AMainPlayerController::GetUIManager() const
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if(LocalPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MainPlayerController ## GetUIManager ## Error : LocalPlayer Is Null"));
		return nullptr;
	}

	return LocalPlayer->GetSubsystem<UUIManager>();
}
