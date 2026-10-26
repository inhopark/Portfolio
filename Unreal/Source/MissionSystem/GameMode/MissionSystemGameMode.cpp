#include "MissionSystemGameMode.h"
#include "Character/User/MissionSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Controllers/MainPlayerController.h"

AMissionSystemGameMode::AMissionSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBlueprintClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if(PlayerPawnBlueprintClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBlueprintClass.Class;
	}

	PlayerControllerClass = AMainPlayerController::StaticClass();
}
