#pragma once

#include "CoreMinimal.h"
#include "Mission/DefenseMinigameMission.h"
#include "MissionAct1Chapter1.generated.h"

UCLASS()
class MISSIONSYSTEM_API UMissionAct1Chapter1 : public UDefenseMinigameMission
{
	GENERATED_BODY()

protected:

	virtual void OnReady() override;
	virtual void OnInProgress() override;
	virtual void OnSucceeded() override;
	virtual void OnFailed() override;

};// end class
