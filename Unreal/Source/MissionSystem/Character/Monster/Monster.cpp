#include "Monster.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Character/User/MissionSystemCharacter.h"

AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionSphere;

	MonsterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonsterMesh"));
	MonsterMesh->SetupAttachment(RootComponent);
	MonsterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if(DefaultMeshAsset.Succeeded() == true)
	{
		MonsterMesh->SetStaticMesh(DefaultMeshAsset.Object);
		MonsterMesh->SetRelativeScale3D(FVector(0.7f));
	}
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnCollisionSphereBeginOverlap);
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DeltaMove = MoveSpeed * DeltaTime;
	AddActorWorldOffset(FVector(-DeltaMove, 0.0f, 0.0f));

	DistanceTraveled += DeltaMove;
	if(DistanceTraveled >= MaxTravelDistance)
	{
		Deactivate();
	}
}

void AMonster::OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMissionSystemCharacter* PlayerCharacter = Cast<AMissionSystemCharacter>(OtherActor);
	if(PlayerCharacter == nullptr)
	{
		return;
	}

	PlayerCharacter->ApplyDamage(DamageAmount);

	Deactivate();
}

void AMonster::ActivateAt(const FVector& SpawnLocation)
{
	SetActorLocation(SpawnLocation);
	DistanceTraveled = 0.0f;
	bIsActive = true;

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
}

void AMonster::Deactivate()
{
	if(bIsActive == false)
	{
		return;
	}
	bIsActive = false;

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);

	OnDeactivated.Broadcast(this);
}
