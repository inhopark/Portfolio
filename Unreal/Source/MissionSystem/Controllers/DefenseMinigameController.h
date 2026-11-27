#pragma once

#include "CoreMinimal.h"
#include "DefenseMinigameController.generated.h"

class AMonsterSpawner;
class AMissionSystemCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefenseMinigameSurvived);

UCLASS()
class MISSIONSYSTEM_API UDefenseMinigameController : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY()
	AMonsterSpawner* MonsterSpawner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Defense")
	float SurvivalDuration = 30.0f;

	FTimerHandle SurvivalTimerHandle;

public:

	UPROPERTY()
	FOnDefenseMinigameSurvived OnSurvived;

public:

	void SetSurvivalDurationOverride(float NewDuration);

	void Start(AMissionSystemCharacter* Player);

	void Stop();

	float GetRemainingTime() const;

	FORCEINLINE float GetTotalDuration() const { return SurvivalDuration; }

private:

	UFUNCTION()
	void HandleTimerExpired();

	AMonsterSpawner* GetOrCreateMonsterSpawner();

};// end class
