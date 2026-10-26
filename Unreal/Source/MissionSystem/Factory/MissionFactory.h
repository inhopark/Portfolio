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

	TMap<EMissionUnique, TFunction<UBaseMission*(UObject*)>> MissionCreators;

public:

	void Initialize();

	UBaseMission* CreateMission(UObject* Outer, EMissionUnique MissionUnique);

private:

	template <typename TMissionClass>
	void RegisterMission(EMissionUnique MissionUnique);

};// end class

template <typename TMissionClass>
void UMissionFactory::RegisterMission(EMissionUnique MissionUnique)
{
	if(MissionCreators.Contains(MissionUnique) == true)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionFactory ## RegisterMission ## Duplicate Mission ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
		return;
	}

	MissionCreators.Add(MissionUnique, [](UObject* Outer) -> UBaseMission*
	{
		return NewObject<TMissionClass>(Outer);
	});
}
