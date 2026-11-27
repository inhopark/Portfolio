#include "MonsterSpawner.h"
#include "Character/Monster/Monster.h"

AMonsterSpawner::AMonsterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	MonsterClass = AMonster::StaticClass();
}

void AMonsterSpawner::StartSpawning(AActor* InTargetPlayer)
{
	TargetPlayer = InTargetPlayer;

	UWorld* World = GetWorld();
	if(World != nullptr)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMonsterSpawner::SpawnMonster, SpawnInterval, true, 0.5f);
	}
}

void AMonsterSpawner::StopSpawning()
{
	UWorld* World = GetWorld();
	if(World != nullptr)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}

	TArray<AMonster*> MonstersToDeactivate = ActiveMonsters;
	for(AMonster* Monster : MonstersToDeactivate)
	{
		if(Monster != nullptr)
		{
			Monster->Deactivate();
		}
	}

	TargetPlayer = nullptr;
}

void AMonsterSpawner::SpawnMonster()
{
	if(TargetPlayer.IsValid() == false || MonsterClass == nullptr)
	{
		return;
	}

	const int32 SpawnCount = FMath::RandRange(MinSpawnCount, MaxSpawnCount);
	for(int32 Index = 0; Index < SpawnCount; ++Index)
	{
		SpawnOneMonster();
	}
}

void AMonsterSpawner::SpawnOneMonster()
{
	AMonster* Monster = AcquireMonster();
	if(Monster == nullptr)
	{
		return;
	}

	const FVector PlayerLocation = TargetPlayer->GetActorLocation();
	const float RandomY = FMath::FRandRange(-SpawnRangeY, SpawnRangeY);
	const FVector SpawnLocation = PlayerLocation + FVector(SpawnDistanceX, RandomY, 0.0f);

	Monster->ActivateAt(SpawnLocation);
	ActiveMonsters.Add(Monster);
}

AMonster* AMonsterSpawner::AcquireMonster()
{
	if(PooledMonsters.Num() > 0)
	{
		return PooledMonsters.Pop();
	}

	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AMonster* NewMonster = World->SpawnActor<AMonster>(MonsterClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	if(NewMonster == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MonsterSpawner ## AcquireMonster ## Monster Spawn Failed ## MonsterClass : %s"), *GetNameSafe(MonsterClass));
		return nullptr;
	}

	NewMonster->OnDeactivated.AddDynamic(this, &AMonsterSpawner::HandleMonsterDeactivated);

	return NewMonster;
}

void AMonsterSpawner::HandleMonsterDeactivated(AMonster* Monster)
{
	if(Monster == nullptr)
	{
		return;
	}

	ActiveMonsters.RemoveSingleSwap(Monster);
	PooledMonsters.Add(Monster);
}
