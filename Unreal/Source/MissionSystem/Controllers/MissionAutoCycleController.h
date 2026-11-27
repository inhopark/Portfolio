#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tickable.h"
#include "Define/MissionDefine.h"
#include "MissionAutoCycleController.generated.h"

class ANPC;
class AMissionSystemCharacter;
class UMissionManager;

UCLASS()
class MISSIONSYSTEM_API UMissionAutoCycleController : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

private:

	bool bIsEnabled = false;

	bool bIsApproachingNPC = false;

	bool bIsDodging = false;

	bool bIsDodgeMovingRight = true;

	FVector DodgeLegStartLocation = FVector::ZeroVector;

	UPROPERTY()
	ANPC* TargetNPC = nullptr;

	FTimerHandle ActionDelayTimerHandle;

	UPROPERTY(EditAnywhere, Category = "AutoCycle")
	float AutoApproachMoveSpeed = 650.0f;

	UPROPERTY(EditAnywhere, Category = "AutoCycle")
	float DodgeSwitchDistance = 150.0f;

	UPROPERTY(EditAnywhere, Category = "AutoCycle")
	float AutoAgreeDelay = 1.5f;

	UPROPERTY(EditAnywhere, Category = "AutoCycle")
	float AutoConfirmDelay = 1.5f;

	UPROPERTY(EditAnywhere, Category = "AutoCycle")
	float AutoCycleSurvivalDuration = 5.0f;

public:

	void Initialize();

	void Deinitialize();

	virtual UWorld* GetWorld() const override;

	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override { return HasAnyFlags(RF_ClassDefaultObject) == true ? ETickableTickType::Never : ETickableTickType::Conditional; }
	virtual bool IsTickable() const override { return bIsApproachingNPC == true || bIsDodging == true; }
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetWorld(); }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UMissionAutoCycleController, STATGROUP_Tickables); }

public:

	void ToggleAutoCycleMode();

private:

	void StartApproachingTargetNPC();

	void StopApproachingTargetNPC();

	void AdvanceTowardTargetNPC();

	void StartDodging();

	void StopDodging();

	void AdvanceDodgeMovement();

	UFUNCTION()
	void HandleMissionStateChanged(EMissionState NewState);

	void TriggerAutoAgree();

	void TriggerAutoConfirm();

	UMissionManager* GetMissionManager() const;

	AMissionSystemCharacter* GetPlayerCharacter() const;

	ANPC* FindNearestNPC(const AMissionSystemCharacter* PlayerCharacter) const;

};// end class
