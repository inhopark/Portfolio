#include "MainPlayerController.h"
#include "Manager/MissionManager.h"
#include "Manager/MissionAutoCycleController.h"
#include "UMG/MainMissionWidget.h"
#include "UMG/MissionResultWidget.h"
#include "UMG/HPWidget.h"
#include "UMG/MissionTimerWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainMissionWidgetInstance = CreateAndAddWidget(MainMissionWidgetClass, TEXT("/Game/UMG/WBP_MainMission.WBP_MainMission_C"));
	MissionResultWidgetInstance = CreateAndAddWidget(MissionResultWidgetClass, TEXT("/Game/UMG/WBP_MainMissionResult.WBP_MainMissionResult_C"));
	HPWidgetInstance = CreateAndAddWidget(HPWidgetClass, TEXT("/Game/UMG/WBP_HP.WBP_HP_C"));
	MissionTimerWidgetInstance = CreateAndAddWidget(MissionTimerWidgetClass, TEXT("/Game/UMG/WBP_MissionTimer.WBP_MissionTimer_C"));

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>();
	if(MissionManager != nullptr)
	{
		MissionManager->RegisterMainMissionWidget(MainMissionWidgetInstance);
		MissionManager->RegisterMissionResultWidgets(MissionResultWidgetInstance);
	}
}

void AMainPlayerController::SetDefenseHUDVisible(bool bVisible)
{
	// 버튼이 없는 순수 표시용 위젯들이므로 HitTestInvisible로 노출해
	// 다른 위젯(미션 결과창 등)의 클릭을 가로채지 않도록 함
	const ESlateVisibility TargetVisibility = bVisible == true ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;

	if(HPWidgetInstance != nullptr)
	{
		HPWidgetInstance->SetVisibility(TargetVisibility);
	}

	if(MissionTimerWidgetInstance != nullptr)
	{
		MissionTimerWidgetInstance->SetVisibility(TargetVisibility);
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
