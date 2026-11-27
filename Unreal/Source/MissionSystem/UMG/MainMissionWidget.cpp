#include "MainMissionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMainMissionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(LeftButton != nullptr)
	{
		LeftButton->OnClicked.AddDynamic(this, &UMainMissionWidget::OnLeftButtonClicked);
	}

	if(RightButton != nullptr)
	{
		RightButton->OnClicked.AddDynamic(this, &UMainMissionWidget::OnRightButtonClicked);
	}
}

void UMainMissionWidget::SetWidgetState(EMissionState MissionState)
{
	UE_LOG(LogTemp, Log, TEXT("## MainMissionWidget ## SetWidgetState ## MissionState : %s"), *UEnum::GetValueAsString(MissionState));

	switch (MissionState)
	{
		case EMissionState::Ready:
			{
				SetMissionTitle(WAIT_FOR_MISSION);
				SetButtonTexts(MISSION_AGREE_BUTTON_TEXT, MISSION_DISAGREE_BUTTON_TEXT);
			}
			break;
		case EMissionState::InProgress:
			{
				SetMissionTitle(MISSION_IN_PROGRESS);
				SetButtonTexts(MISSION_SUCCESS_BUTTON_TEXT, MISSION_FAILED_BUTTON_TEXT);
			}
			break;
		default:
			{
			}
			break;
	}

	const bool bShowButtons = MissionState == EMissionState::Ready || MissionState == EMissionState::InProgress;
	const ESlateVisibility ButtonVisibility = bShowButtons == true ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	if(LeftButton != nullptr)
	{
		LeftButton->SetVisibility(ButtonVisibility);
	}
	if(RightButton != nullptr)
	{
		RightButton->SetVisibility(ButtonVisibility);
	}
}

void UMainMissionWidget::SetMissionTitle(const FText& NewTitle)
{
	if(MissionTitleText != nullptr)
	{
		MissionTitleText->SetText(NewTitle);
	}
}

void UMainMissionWidget::SetButtonTexts(const FText& LeftText, const FText& RightText)
{
	if(LeftButtonTextBlock != nullptr)
	{
		LeftButtonTextBlock->SetVisibility(ESlateVisibility::Visible);
		LeftButtonTextBlock->SetText(LeftText);
	}
	if(RightButtonTextBlock != nullptr)
	{
		RightButtonTextBlock->SetVisibility(ESlateVisibility::Visible);
		RightButtonTextBlock->SetText(RightText);
	}
}

void UMainMissionWidget::OnLeftButtonClicked()
{
	OnMissionButtonAction.Broadcast(EMissionButtonAction::Left);
}

void UMainMissionWidget::OnRightButtonClicked()
{
	OnMissionButtonAction.Broadcast(EMissionButtonAction::Right);
}
