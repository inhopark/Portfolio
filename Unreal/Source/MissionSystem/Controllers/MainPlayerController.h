#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MainPlayerController.generated.h"

class UMissionAutoCycleController;

UCLASS()
class MISSIONSYSTEM_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TObjectPtr<UMissionAutoCycleController> AutoCycleController = nullptr;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	void SetDefenseHUDVisible(bool bVisible);

	UFUNCTION(Exec)
	void ToggleAutoCycleMode();

private:

	void InitManager();

	template <typename TSubsystemClass>
	TSubsystemClass* GetSubsystem() const;

};// end class

template <typename TSubsystemClass>
TSubsystemClass* AMainPlayerController::GetSubsystem() const
{
	static_assert(TIsDerivedFrom<TSubsystemClass, UGameInstanceSubsystem>::IsDerived, "TSubsystemClass must derive from UGameInstanceSubsystem");

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MainPlayerController ## GetSubsystem ## Error : GameInstance Is Null ## Subsystem : %s"), *TSubsystemClass::StaticClass()->GetName());
		return nullptr;
	}

	return GameInstance->GetSubsystem<TSubsystemClass>();
}
