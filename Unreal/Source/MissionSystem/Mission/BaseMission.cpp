#include "BaseMission.h"
#include "Character/User/MissionSystemCharacter.h"

UBaseMission::UBaseMission()
{
}

void UBaseMission::Initialize()
{
	SetState(EMissionState::Ready);
}

void UBaseMission::OnReady()
{
}

void UBaseMission::OnInProgress()
{
}

void UBaseMission::OnSucceeded()
{
}

void UBaseMission::OnFailed()
{
}

void UBaseMission::AgreeMission(AMissionSystemCharacter* Player)
{
	MissionPlayer = Player;

	// 미션 진행 중으로 변경.
	SetState(EMissionState::InProgress);
}

void UBaseMission::DisagreeMission()
{
}

void UBaseMission::SuccessMission()
{
	SetState(EMissionState::Succeeded);
}

void UBaseMission::FailedMission()
{
	SetState(EMissionState::Failed);
}

AMissionSystemCharacter* UBaseMission::GetMissionPlayer() const
{
	return MissionPlayer.Get();
}

bool UBaseMission::SetState(EMissionState NewState)
{
	if(MissionState == NewState)
	{
		UE_LOG(LogTemp, Log, TEXT("## BaseMission ## SetState ## Warning : Already In State ## Mission : %s ## State : %s"), *GetClass()->GetName(), *UEnum::GetValueAsString(NewState));
		return false;
	}

	MissionState = NewState;

	UE_LOG(LogTemp, Log, TEXT("## BaseMission ## SetState ## Mission : %s ## State : %s"), *GetClass()->GetName(), *UEnum::GetValueAsString(MissionState));

	switch (MissionState)
	{
		case EMissionState::Ready:
			{
				OnReady();
			}
			break;
		case EMissionState::InProgress:
			{
				OnInProgress();
			}
			break;
		case EMissionState::Succeeded:
			{
				OnSucceeded();
			}
			break;
		case EMissionState::Failed:
			{
				OnFailed();
			}
			break;
		default:
			{
			}
			break;
	}

	OnMissionStateChanged.Broadcast(NewState);

	return true;
}
