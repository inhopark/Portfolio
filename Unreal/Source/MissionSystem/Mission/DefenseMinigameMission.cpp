#include "DefenseMinigameMission.h"
#include "Controllers/DefenseMinigameController.h"
#include "Character/User/MissionSystemCharacter.h"

void UDefenseMinigameMission::InitData()
{
	Super::InitData();

	// 미니게임 컨트롤러 생성 및 생존 성공 이벤트 바인딩
	DefenseMinigame = NewObject<UDefenseMinigameController>(this);
	DefenseMinigame->OnSurvived.AddDynamic(this, &UDefenseMinigameMission::HandleMinigameSurvived);
}

void UDefenseMinigameMission::OnInProgress()
{
	Super::OnInProgress();

	if(DefenseMinigame == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## DefenseMinigameMission ## OnInProgress ## DefenseMinigame Not Initialized ## Mission : %s"), *GetClass()->GetName());
		return;
	}

	DefenseMinigame->Start(GetMissionPlayer());
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

void UDefenseMinigameMission::HandleMinigameSurvived()
{
	SuccessMission();
}
