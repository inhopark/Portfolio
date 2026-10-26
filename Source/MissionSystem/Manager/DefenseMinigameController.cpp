#include "DefenseMinigameController.h"
#include "Spawner/MonsterSpawner.h"
#include "Character/User/MissionSystemCharacter.h"

void UDefenseMinigameController::Start(AMissionSystemCharacter* Player)
{
	if(Player == nullptr)
	{
		return;
	}

	AMonsterSpawner* Spawner = GetOrCreateMonsterSpawner();
	if(Spawner != nullptr)
	{
		Spawner->StartSpawning(Player);
	}

	UWorld* World = GetWorld();
	if(World != nullptr)
	{
		World->GetTimerManager().SetTimer(SurvivalTimerHandle, this, &UDefenseMinigameController::HandleTimerExpired, SurvivalDuration, false);
	}
}

void UDefenseMinigameController::SetSurvivalDurationOverride(float NewDuration)
{
	if(NewDuration <= 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("## DefenseMinigameController ## SetSurvivalDurationOverride ## Warning : Invalid Duration ## NewDuration : %f"), NewDuration);
		return;
	}

	SurvivalDuration = NewDuration;

	UE_LOG(LogTemp, Log, TEXT("## DefenseMinigameController ## SetSurvivalDurationOverride ## SurvivalDuration : %f"), SurvivalDuration);
}

void UDefenseMinigameController::Stop()
{
	if(MonsterSpawner != nullptr)
	{
		MonsterSpawner->StopSpawning();
	}

	UWorld* World = GetWorld();
	if(World != nullptr)
	{
		World->GetTimerManager().ClearTimer(SurvivalTimerHandle);
	}
}

float UDefenseMinigameController::GetRemainingTime() const
{
	const UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return -1.0f;
	}

	return World->GetTimerManager().GetTimerRemaining(SurvivalTimerHandle);
}

void UDefenseMinigameController::HandleTimerExpired()
{
	OnSurvived.Broadcast();
}

AMonsterSpawner* UDefenseMinigameController::GetOrCreateMonsterSpawner()
{
	if(MonsterSpawner != nullptr)
	{
		return MonsterSpawner;
	}

	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return nullptr;
	}

	MonsterSpawner = World->SpawnActor<AMonsterSpawner>();
	if(MonsterSpawner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## DefenseMinigameController ## GetOrCreateMonsterSpawner ## MonsterSpawner Spawn Failed"));
	}

	return MonsterSpawner;
}
