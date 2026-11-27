#include "MainPlayerController.h"
#include "Manager/MissionManager.h"
#include "Manager/UIManager.h"
#include "Controllers/MissionAutoCycleController.h"
#include "UMG/MainMissionWidget.h"
#include "UMG/MissionResultWidget.h"
#include "UMG/HPWidget.h"
#include "UMG/MissionTimerWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitManager();

	AutoCycleController = NewObject<UMissionAutoCycleController>(this);
	AutoCycleController->Initialize();
}

void AMainPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(AutoCycleController != nullptr)
	{
		AutoCycleController->Deinitialize();
		AutoCycleController = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AMainPlayerController::SetDefenseHUDVisible(bool bVisible)
{
	UUIManager* UIManager = GetSubsystem<UUIManager>();
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
	if(AutoCycleController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MainPlayerController ## ToggleAutoCycleMode ## Error : AutoCycleController Is Null"));
		return;
	}

	AutoCycleController->ToggleAutoCycleMode();
}

void AMainPlayerController::InitManager()
{
	UMainMissionWidget* MainMissionWidget = nullptr;
	UMissionResultWidget* MissionResultWidget = nullptr;

	UUIManager* UIManager = GetSubsystem<UUIManager>();
	if(UIManager != nullptr)
	{
		UIManager->InitializeUI(this);
		MainMissionWidget = UIManager->GetUI<UMainMissionWidget>(EUIType::MainMission);
		MissionResultWidget = UIManager->GetUI<UMissionResultWidget>(EUIType::MissionResult);
	}

	UMissionManager* MissionManager = GetSubsystem<UMissionManager>();
	if(MissionManager != nullptr)
	{
		MissionManager->RegisterMainMissionWidget(MainMissionWidget);
		MissionManager->RegisterMissionResultWidget(MissionResultWidget);
	}
}
