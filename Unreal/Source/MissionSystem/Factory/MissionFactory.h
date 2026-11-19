#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Define/MissionDefine.h"
#include "Mission/BaseMission.h"
#include "MissionFactory.generated.h"

UCLASS()
class MISSIONSYSTEM_API UMissionFactory : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<EMissionUnique, TSubclassOf<UBaseMission>> MissionClasses;

public:

	void Initialize();

	UBaseMission* CreateMission(UObject* Outer, EMissionUnique MissionUnique);

private:

	void RegisterMission(EMissionUnique MissionUnique, TSubclassOf<UBaseMission> MissionClass);

};// end class
