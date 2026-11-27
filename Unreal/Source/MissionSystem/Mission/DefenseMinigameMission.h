#pragma once

#include "CoreMinimal.h"
#include "Mission/BaseMission.h"
#include "DefenseMinigameMission.generated.h"

UCLASS(Abstract)
class MISSIONSYSTEM_API UDefenseMinigameMission : public UBaseMission
{
	GENERATED_BODY()

private:

	UPROPERTY()
	class UDefenseMinigameController* DefenseMinigame = nullptr;

protected:

	virtual void InitData() override;

	virtual void OnInProgress() override;

	virtual void OnSucceeded() override;

	virtual void OnFailed() override;

public:

	virtual float GetMinigameRemainingTime() const override;

	virtual float GetMinigameTotalDuration() const override;

	FORCEINLINE class UDefenseMinigameController* GetDefenseMinigameController() const { return DefenseMinigame; }

private:

	UFUNCTION()
	void HandleMinigameSurvived();

};// end class
