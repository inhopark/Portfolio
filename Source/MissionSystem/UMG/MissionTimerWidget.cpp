#include "MissionTimerWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Manager/MissionManager.h"
#include "Kismet/GameplayStatics.h"

void UMissionTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>();
	if(MissionManager == nullptr)
	{
		return;
	}

	const float RemainingTime = MissionManager->GetMissionRemainingTime();
	if(RemainingTime >= 0.0f)
	{
		SetRemainingTime(RemainingTime, MissionManager->GetMissionTotalDuration());
	}
}

void UMissionTimerWidget::SetRemainingTime(float RemainingTime, float TotalDuration)
{
	if(TimerBar != nullptr)
	{
		const float TimerPercent = TotalDuration > 0.0f ? RemainingTime / TotalDuration : 0.0f;
		TimerBar->SetPercent(TimerPercent);
	}

	if(TimerText != nullptr)
	{
		TimerText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingTime)));
	}
}
