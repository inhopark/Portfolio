#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

class AMonster;

UCLASS()
class MISSIONSYSTEM_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	TSubclassOf<AMonster> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	float SpawnInterval = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	float SpawnDistanceX = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	float SpawnRangeY = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	int32 MinSpawnCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Spawn")
	int32 MaxSpawnCount = 3;

private:

	TWeakObjectPtr<AActor> TargetPlayer;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY()
	TArray<AMonster*> PooledMonsters;

	UPROPERTY()
	TArray<AMonster*> ActiveMonsters;

public:

	AMonsterSpawner();

public:

	void StartSpawning(AActor* InTargetPlayer);

	void StopSpawning();

private:

	void SpawnMonster();

	void SpawnOneMonster();

	AMonster* AcquireMonster();

	UFUNCTION()
	void HandleMonsterDeactivated(AMonster* Monster);

};// end class
