#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Define/MissionDefine.h"
#include "MissionResultWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionResultConfirmed);

// 미션 성공/실패 결과창. WBP_MainMissionSuccess, WBP_MainMissionFail이 이 클래스를 사용하며,
// 위젯 트리는 WBP_MainMission을 복제해 만들어졌기 때문에 바인딩 이름을 그대로 유지함.
UCLASS()
class MISSIONSYSTEM_API UMissionResultWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MissionTitleText = nullptr;   // 결과 제목 텍스트 ("미션 성공"/"미션 실패")

	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton = nullptr;            // 확인 버튼으로 재사용

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LeftButtonTextBlock = nullptr;  // 확인 버튼 텍스트

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
