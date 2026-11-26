#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUIManager;

UCLASS()
class MISSIONSYSTEM_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	// 디펜스 모드 진입/종료 시 HP・타이머 위젯을 함께 표시/숨김
	void SetDefenseHUDVisible(bool bVisible);

	// 콘솔 명령("ToggleAutoCycleMode")으로 자동 사이클 모드를 켜고 끔.
	// 자동화 로직은 전부 UMissionAutoCycleController(GameInstanceSubsystem)가 갖고 있고,
	// PlayerController는 콘솔 Exec 디스패치 체인에 포함되는 진입점 역할만 함
	UFUNCTION(Exec)
	void ToggleAutoCycleMode();

private:

	UUIManager* GetUIManager() const;

};// end class
