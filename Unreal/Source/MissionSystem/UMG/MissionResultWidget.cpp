#include "MissionResultWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Define/MissionDefine.h"

void UMissionResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(LeftButton != nullptr)
	{
		LeftButton->OnClicked.AddDynamic(this, &UMissionResultWidget::OnConfirmButtonClicked);
	}

	if(LeftButtonTextBlock != nullptr)
	{
		LeftButtonTextBlock->SetText(MISSION_CONFIRM_BUTTON_TEXT);
	}
}

void UMissionResultWidget::SetResultState(EMissionState NewState)
{
	ResultState = NewState;

	UE_LOG(LogTemp, Log, TEXT("## MissionResultWidget ## SetResultState ## ResultState : %s"), *UEnum::GetValueAsString(ResultState));

	if(ResultState == EMissionState::Succeeded)
	{
		SetResultTitle(MISSION_SUCCESS);
	}
	else if(ResultState == EMissionState::Failed)
	{
		SetResultTitle(MISSION_FAILED);
	}
}

void UMissionResultWidget::OnConfirmButtonClicked()
{
	OnConfirmed.Broadcast();
}

void UMissionResultWidget::SetResultTitle(const FText& NewTitle)
{
	if(MissionTitleText != nullptr)
	{
		MissionTitleText->SetText(NewTitle);
	}
}
