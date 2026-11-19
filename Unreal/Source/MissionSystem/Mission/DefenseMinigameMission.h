#pragma once

#include "CoreMinimal.h"
#include "Mission/BaseMission.h"
#include "DefenseMinigameMission.generated.h"

// 쿼터뷰 디펜스 미니게임(몬스터 웨이브 생존)으로 진행되는 미션들의 공통 베이스.
// UDefenseMinigameController의 시작/종료를 State 훅(On*)에 연결해,
// 미니게임의 소유권과 생명주기를 MissionManager가 아니라 미션 자신이 갖도록 함.
UCLASS(Abstract)
class MISSIONSYSTEM_API UDefenseMinigameMission : public UBaseMission
{
	GENERATED_BODY()

private:

	UPROPERTY()
	class UDefenseMinigameController* DefenseMinigame = nullptr;

protected:

	// 미니게임 컨트롤러 생성 및 바인딩
	virtual void InitData() override;

	virtual void OnInProgress() override;

	virtual void OnSucceeded() override;

	virtual void OnFailed() override;

public:

	virtual float GetMinigameRemainingTime() const override;

	virtual float GetMinigameTotalDuration() const override;

	// Initialize()에서 생성된 미니게임 컨트롤러 반환.
	// public인 이유: 자동 사이클 모드가 Agree 처리(OnInProgress -> Start()) 전에
	// 생존 시간을 미리 단축 설정할 수 있도록 컨트롤러 인스턴스에 먼저 접근해야 하기 때문
	FORCEINLINE class UDefenseMinigameController* GetDefenseMinigameController() const { return DefenseMinigame; }

private:

	UFUNCTION()
	void HandleMinigameSurvived();

};// end class
