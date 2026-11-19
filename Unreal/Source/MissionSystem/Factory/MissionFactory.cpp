#include "MissionFactory.h"
#include "Mission/BaseMission.h"
#include "Mission/MissionAct1Chapter1.h"
#include "Mission/MissionAct1Chapter2.h"

void UMissionFactory::Initialize()
{
	RegisterMission(EMissionUnique::Act1Chapter1, UMissionAct1Chapter1::StaticClass());
	RegisterMission(EMissionUnique::Act1Chapter2, UMissionAct1Chapter2::StaticClass());
}

UBaseMission* UMissionFactory::CreateMission(UObject* Outer, EMissionUnique MissionUnique)
{
	const TSubclassOf<UBaseMission>* MissionClass = MissionClasses.Find(MissionUnique);
	if(MissionClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionFactory ## CreateMission ## Mission Not Registered ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
		return nullptr;
	}

	return NewObject<UBaseMission>(Outer, *MissionClass);
}

void UMissionFactory::RegisterMission(EMissionUnique MissionUnique, TSubclassOf<UBaseMission> MissionClass)
{
	if(MissionClasses.Contains(MissionUnique) == true)
	{
		UE_LOG(LogTemp, Error, TEXT("## MissionFactory ## RegisterMission ## Duplicate Mission ## MissionUnique : %s"), *UEnum::GetValueAsString(MissionUnique));
		return;
	}

	MissionClasses.Add(MissionUnique, MissionClass);
}
