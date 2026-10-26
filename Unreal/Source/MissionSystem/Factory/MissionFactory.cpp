#include "MissionFactory.h"
#include "Mission/BaseMission.h"
#include "Mission/MissionAct1Chapter1.h"
#include "Mission/MissionAct1Chapter2.h"

void UMissionFactory::Initialize()
{
	RegisterMission<UMissionAct1Chapter1>(EMissionUnique::Act1Chapter1);
	RegisterMission<UMissionAct1Chapter2>(EMissionUnique::Act1Chapter2);
}

UBaseMission* UMissionFactory::CreateMission(UObject* Outer, EMissionUnique MissionUnique)
{
	const TFunction<UBaseMission*(UObject*)>* MissionCreator = MissionCreators.Find(MissionUnique);
	if(MissionCreator == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionFactory ## CreateMission ## Mission Not Registered ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
		return nullptr;
	}

	return (*MissionCreator)(Outer);
}
