#pragma once

#include "CoreMinimal.h"
#include "UMG/BaseWidget.h"
#include "Define/MissionDefine.h"
#include "MainMissionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionButtonAction, EMissionButtonAction, Action);

UCLASS()
class MISSIONSYSTEM_API UMainMissionWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MissionTitleText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UButton* RightButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeftButtonTextBlock = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RightButtonTextBlock = nullptr;

public:

	UPROPERTY()
	FOnMissionButtonAction OnMissionButtonAction;

public:

	virtual void NativeConstruct() override;

public:

	void SetWidgetState(EMissionState MissionState);
	void SetMissionTitle(const FText& NewTitle);
	void SetButtonTexts(const FText& LeftText, const FText& RightText);

	UFUNCTION()
	void OnLeftButtonClicked();

	UFUNCTION()
	void OnRightButtonClicked();

};// end class
