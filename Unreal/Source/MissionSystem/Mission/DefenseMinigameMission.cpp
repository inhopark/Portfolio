#include "DefenseMinigameMission.h"
#include "Manager/DefenseMinigameController.h"
#include "Character/User/MissionSystemCharacter.h"

void UDefenseMinigameMission::OnInProgress()
{
	Super::OnInProgress();

	GetOrCreateDefenseMinigameController()->Start(GetMissionPlayer());
}

void UDefenseMinigameMission::OnSucceeded()
{
	Super::OnSucceeded();

	if(DefenseMinigame != nullptr)
	{
		DefenseMinigame->Stop();
	}
}

void UDefenseMinigameMission::OnFailed()
{
	Super::OnFailed();

	if(DefenseMinigame != nullptr)
	{
		DefenseMinigame->Stop();
	}
}

float UDefenseMinigameMission::GetMinigameRemainingTime() const
{
	if(DefenseMinigame == nullptr)
	{
		return -1.0f;
	}

	return DefenseMinigame->GetRemainingTime();
}

float UDefenseMinigameMission::GetMinigameTotalDuration() const
{
	if(DefenseMinigame == nullptr)
	{
		return 0.0f;
	}

	return DefenseMinigame->GetTotalDuration();
}

UDefenseMinigameController* UDefenseMinigameMission::GetOrCreateDefenseMinigameController()
{
	if(DefenseMinigame == nullptr)
	{
		DefenseMinigame = NewObject<UDefenseMinigameController>(this);
		DefenseMinigame->OnSurvived.AddDynamic(this, &UDefenseMinigameMission::HandleMinigameSurvived);
	}

	return DefenseMinigame;
}

void UDefenseMinigameMission::HandleMinigameSurvived()
{
	SuccessMission();
}
