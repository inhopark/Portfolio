#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MissionSystemCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType)
enum class ECharacterPlayMode : uint8
{
	Normal,
	DefenseMode
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurrentHP, float, MaxHP);

UCLASS(config=Game)
class AMissionSystemCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction = nullptr;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayMode")
	ECharacterPlayMode CurrentPlayMode = ECharacterPlayMode::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Defense")
	float DefenseCameraDistance = 1300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Defense")
	float DefenseCameraForwardOffset = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayMode")
	float NormalModeMoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayMode")
	float DefenseModeMoveSpeed = 850.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	float CurrentHP = 100.0f;

public:

	UPROPERTY(BlueprintAssignable, Category = "HP")
	FOnHPChanged OnHPChanged;

public:

	AMissionSystemCharacter();

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetPlayMode(ECharacterPlayMode NewMode);
	FORCEINLINE ECharacterPlayMode GetPlayMode() const { return CurrentPlayMode; }

	void ApplyDamage(float DamageAmount);

	void ResetHP();

	void ResetPlayer();

	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

private:

	void MoveToPlayerStart();

};// end class
