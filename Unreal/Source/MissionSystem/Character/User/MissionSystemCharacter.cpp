#include "MissionSystemCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Manager/MissionManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMissionSystemCharacter::AMissionSystemCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = NormalModeMoveSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CurrentHP = MaxHP;
}

void AMissionSystemCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CameraBoom == nullptr)
	{
		return;
	}

	if(CurrentPlayMode == ECharacterPlayMode::DefenseMode)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, DefenseCameraDistance, DeltaTime, 3.0f);
		const FRotator TargetRotation(-55.0f, 0.0f, 0.0f);
		CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), TargetRotation, DeltaTime, 3.0f));

		const FVector TargetOffset(DefenseCameraForwardOffset, 0.0f, 0.0f);
		CameraBoom->TargetOffset = FMath::VInterpTo(CameraBoom->TargetOffset, TargetOffset, DeltaTime, 3.0f);

		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, 400.0f, DeltaTime, 4.0f);
		CameraBoom->TargetOffset = FMath::VInterpTo(CameraBoom->TargetOffset, FVector::ZeroVector, DeltaTime, 4.0f);
	}
}

void AMissionSystemCharacter::SetPlayMode(ECharacterPlayMode NewMode)
{
	CurrentPlayMode = NewMode;

	UE_LOG(LogTemplateCharacter, Log, TEXT("## MissionSystemCharacter ## SetPlayMode ## PlayMode : %s"), *UEnum::GetValueAsString(CurrentPlayMode));

	if(CameraBoom == nullptr || GetCharacterMovement() == nullptr)
	{
		return;
	}

	if(CurrentPlayMode == ECharacterPlayMode::DefenseMode)
	{
		CameraBoom->bUsePawnControlRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = DefenseModeMoveSpeed;
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else
	{
		CameraBoom->bUsePawnControlRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = NormalModeMoveSpeed;
	}
}

void AMissionSystemCharacter::ApplyDamage(float DamageAmount)
{
	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.0f, MaxHP);

	UE_LOG(LogTemplateCharacter, Log, TEXT("## MissionSystemCharacter ## ApplyDamage ## DamageAmount : %f ## CurrentHP : %f"), DamageAmount, CurrentHP);

	OnHPChanged.Broadcast(CurrentHP, MaxHP);

	if(CurrentHP > 0.0f)
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>();
	if(MissionManager != nullptr)
	{
		MissionManager->HandlePlayerDefeated();
	}
}

void AMissionSystemCharacter::ResetHP()
{
	CurrentHP = MaxHP;

	UE_LOG(LogTemplateCharacter, Log, TEXT("## MissionSystemCharacter ## ResetHP ## CurrentHP : %f"), CurrentHP);

	OnHPChanged.Broadcast(CurrentHP, MaxHP);
}

void AMissionSystemCharacter::ResetPlayer()
{
	MoveToPlayerStart();
	SetPlayMode(ECharacterPlayMode::Normal);
	ResetHP();

	UE_LOG(LogTemplateCharacter, Log, TEXT("## MissionSystemCharacter ## ResetPlayer ## Location : %s"), *GetActorLocation().ToString());
}

void AMissionSystemCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if(Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMissionSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("## MissionSystemCharacter ## SetupPlayerInputComponent ## Enhanced Input Component Not Found ## Actor : %s"), *GetNameSafe(this));
		return;
	}

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMissionSystemCharacter::Move);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMissionSystemCharacter::Look);
}

void AMissionSystemCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller == nullptr)
	{
		return;
	}

	if(CurrentPlayMode == ECharacterPlayMode::DefenseMode)
	{
		const FVector RightDirection(0.0f, 1.0f, 0.0f);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMissionSystemCharacter::Look(const FInputActionValue& Value)
{
	if(CurrentPlayMode == ECharacterPlayMode::DefenseMode)
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMissionSystemCharacter::MoveToPlayerStart()
{
	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return;
	}

	AGameModeBase* GameMode = World->GetAuthGameMode();
	if(GameMode == nullptr)
	{
		return;
	}

	AController* OwnerController = GetController();
	if(OwnerController == nullptr)
	{
		return;
	}

	AActor* PlayerStart = GameMode->FindPlayerStart(OwnerController);
	if(PlayerStart == nullptr)
	{
		UE_LOG(LogTemplateCharacter, Log, TEXT("## MissionSystemCharacter ## MoveToPlayerStart ## Warning : PlayerStart Not Found"));
		return;
	}

	SetActorLocationAndRotation(PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), false, nullptr, ETeleportType::TeleportPhysics);
}
