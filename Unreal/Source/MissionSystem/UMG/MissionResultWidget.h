#pragma once

#include "CoreMinimal.h"
#include "UMG/BaseWidget.h"
#include "Define/MissionDefine.h"
#include "MissionResultWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionResultConfirmed);

UCLASS()
class MISSIONSYSTEM_API UMissionResultWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MissionTitleText = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeftButtonTextBlock = nullptr;

private:

	EMissionState ResultState = EMissionState::None;

public:

	UPROPERTY()
	FOnMissionResultConfirmed OnConfirmed;

public:

	virtual void NativeConstruct() override;

public:

	void SetResultState(EMissionState NewState);

	UFUNCTION()
	void OnConfirmButtonClicked();

private:

	void SetResultTitle(const FText& NewTitle);

};// end class
