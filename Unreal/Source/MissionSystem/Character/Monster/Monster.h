#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class AMonster;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterDeactivated, AMonster*, Monster);

UCLASS()
class MISSIONSYSTEM_API AMonster : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	USphereComponent* CollisionSphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	UStaticMeshComponent* MonsterMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MoveSpeed = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MaxTravelDistance = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float DamageAmount = 10.0f;

private:

	float DistanceTraveled = 0.0f;

	bool bIsActive = false;

public:

	UPROPERTY()
	FOnMonsterDeactivated OnDeactivated;

public:

	AMonster();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	void ActivateAt(const FVector& SpawnLocation);

	void Deactivate();

};// end class
