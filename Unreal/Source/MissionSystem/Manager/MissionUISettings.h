#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Define/UIDefine.h"
#include "MissionUISettings.generated.h"

class UBaseWidget;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Mission UI"))
class MISSIONSYSTEM_API UMissionUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "UI")
	TMap<EUIType, TSoftClassPtr<UBaseWidget>> WidgetClasses;

};// end class
