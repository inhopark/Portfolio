#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/MissionDefine.h"
#include "MainMissionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionButtonAction, EMissionButtonAction, Action);

UCLASS()
class MISSIONSYSTEM_API UMainMissionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MissionTitleText = nullptr;  // 미션 제목 텍스트

	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton = nullptr;           // 왼쪽 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* RightButton = nullptr;          // 오른쪽 버튼

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeftButtonTextBlock = nullptr;   // 왼쪽 버튼 텍스트
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RightButtonTextBlock = nullptr;  // 오른쪽 버튼 텍스트

public:

	UPROPERTY()
	FOnMissionButtonAction OnMissionButtonAction;

public:

	virtual void NativeConstruct() override;

public:

	void SetWidgetState(EMissionState MissionState);
	void SetMissionTitle(const FText& NewTitle);  // 미션 제목 설정
	void SetButtonTexts(const FText& LeftText, const FText& RightText);  // 버튼 텍스트 설정

	UFUNCTION()
	void OnLeftButtonClicked();

	UFUNCTION()
	void OnRightButtonClicked();

};// end class
