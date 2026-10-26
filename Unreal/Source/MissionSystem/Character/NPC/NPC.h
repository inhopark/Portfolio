#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Define/MissionDefine.h"
#include "NPC.generated.h"

UCLASS()
class MISSIONSYSTEM_API ANPC : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	EMissionUnique MissionUnique = EMissionUnique::Act1Chapter1;

public:

	ANPC();

protected:

	virtual void BeginPlay() override;

public:

	FORCEINLINE EMissionUnique GetMissionUnique() const { return MissionUnique; }

private:

	UFUNCTION()
	void OnUserCheckOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnUserCheckOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};// end class
