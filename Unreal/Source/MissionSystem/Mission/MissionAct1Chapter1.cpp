#include "MissionAct1Chapter1.h"

void UMissionAct1Chapter1::OnReady()
{
	Super::OnReady();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter1 ## OnReady ## 준비 상태 진입"));
}

void UMissionAct1Chapter1::OnInProgress()
{
	Super::OnInProgress();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter1 ## OnInProgress ## 진행 상태 진입"));
}

void UMissionAct1Chapter1::OnSucceeded()
{
	Super::OnSucceeded();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter1 ## OnSucceeded ## 성공 상태 진입"));
}

void UMissionAct1Chapter1::OnFailed()
{
	Super::OnFailed();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter1 ## OnFailed ## 실패 상태 진입"));
}
