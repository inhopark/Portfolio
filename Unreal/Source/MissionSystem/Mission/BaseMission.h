#pragma once

#include "CoreMinimal.h"
#include "Define/MissionDefine.h"
#include "BaseMission.generated.h"

class AMissionSystemCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionStateChanged, EMissionState, NewState);

UCLASS()
class MISSIONSYSTEM_API UBaseMission : public UObject
{
	GENERATED_BODY()

private:

	EMissionState MissionState = EMissionState::None;

protected:

	TWeakObjectPtr<AMissionSystemCharacter> MissionPlayer;

public:

	UPROPERTY()
	FOnMissionStateChanged OnMissionStateChanged;

public:

	UBaseMission();

	void Initialize();

protected:

	virtual void InitData();

	virtual void OnReady();

	virtual void OnInProgress();

	virtual void OnSucceeded();

	virtual void OnFailed();

public:

	virtual void AgreeMission(AMissionSystemCharacter* Player);

	virtual void DisagreeMission();

	virtual void SuccessMission();

	virtual void FailedMission();

	FORCEINLINE EMissionState GetMissionState() const { return MissionState; }

	virtual float GetMinigameRemainingTime() const { return -1.0f; }

	virtual float GetMinigameTotalDuration() const { return 0.0f; }

protected:

	AMissionSystemCharacter* GetMissionPlayer() const;

private:

	bool SetState(EMissionState NewState);

};// end class
