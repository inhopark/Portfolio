#include "MissionAct1Chapter2.h"

void UMissionAct1Chapter2::OnReady()
{
	Super::OnReady();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter2 ## OnReady ## 준비 상태 진입"));
}

void UMissionAct1Chapter2::OnInProgress()
{
	Super::OnInProgress();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter2 ## OnInProgress ## 진행 상태 진입"));
}

void UMissionAct1Chapter2::OnSucceeded()
{
	Super::OnSucceeded();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter2 ## OnSucceeded ## 성공 상태 진입"));
}

void UMissionAct1Chapter2::OnFailed()
{
	Super::OnFailed();

	UE_LOG(LogTemp, Log, TEXT("## MissionAct1Chapter2 ## OnFailed ## 실패 상태 진입"));
}
