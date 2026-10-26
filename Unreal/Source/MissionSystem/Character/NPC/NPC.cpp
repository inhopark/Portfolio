#include "NPC.h"
#include "Character/User/MissionSystemCharacter.h"
#include "Components/SphereComponent.h"
#include "Manager/MissionManager.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	USphereComponent* SphereComponent = FindComponentByClass<USphereComponent>();
	if(SphereComponent != nullptr)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnUserCheckOverlapBegin);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnUserCheckOverlapEnd);
	}
}

void ANPC::OnUserCheckOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMissionSystemCharacter* PlayerCharacter = Cast<AMissionSystemCharacter>(OtherActor);
	if(PlayerCharacter == nullptr)
	{
		return;
	}

	// 미션 요청 UI 띄우는 로직.
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>();
	if(MissionManager != nullptr)
	{
		MissionManager->ShowMainMissionWidget(MissionUnique, this);
	}
}

void ANPC::OnUserCheckOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AMissionSystemCharacter* PlayerCharacter = Cast<AMissionSystemCharacter>(OtherActor);
	if(PlayerCharacter == nullptr)
	{
		return;
	}

	// 미션 요청 UI 제거하는 로직.
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance == nullptr)
	{
		return;
	}

	UMissionManager* MissionManager = GameInstance->GetSubsystem<UMissionManager>();
	if(MissionManager != nullptr)
	{
		MissionManager->HideMainMissionWidget();
	}
}
