#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Define/MissionDefine.h"
#include "Mission/BaseMission.h"
#include "MissionManager.generated.h"

UCLASS()
class MISSIONSYSTEM_API UMissionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

	UPROPERTY()
	class UMissionFactory* MissionFactory = nullptr;

	UPROPERTY()
	class UBaseMission* CurrentMission = nullptr;

	UPROPERTY()
	class UMainMissionWidget* MainMissionWidget = nullptr;

	UPROPERTY()
	class UMissionResultWidget* MissionResultWidget = nullptr;

	UPROPERTY()
	class ANPC* CurrentMissionNPC = nullptr;

public:

	UPROPERTY()
	FOnMissionStateChanged OnAnyMissionStateChanged;

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	void RegisterMainMissionWidget(class UMainMissionWidget* Widget);

	void RegisterMissionResultWidget(class UMissionResultWidget* ResultWidget);

	void ShowMainMissionWidget(EMissionUnique MissionUnique, class ANPC* InNPC = nullptr);

	void HideMainMissionWidget();

	UFUNCTION()
	void HandleMissionButtonAction(EMissionButtonAction Action);

	void HandlePlayerDefeated();

	UFUNCTION()
	void HandleMissionResultConfirmed();

	float GetMissionRemainingTime() const;

	float GetMissionTotalDuration() const;

	FORCEINLINE UBaseMission* GetCurrentMission() const { return CurrentMission; }

private:

	void RequestMission(EMissionUnique MissionUnique);

	bool IsMissionRequest();

	UFUNCTION()
	void HandleMissionStateChanged(EMissionState NewState);

	void SetUICursorMode(bool bShow);

	void ShowMissionResultWidget(EMissionState NewState);

	void ClearCurrentMission();

	void SetDefenseHUDVisible(bool bVisible);

	class AMissionSystemCharacter* GetPlayerCharacter() const;
	class AMainPlayerController* GetMainPlayerController() const;

};// end class
