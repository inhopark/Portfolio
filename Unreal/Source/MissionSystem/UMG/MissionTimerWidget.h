#pragma once

#include "CoreMinimal.h"
#include "UMG/BaseWidget.h"
#include "MissionTimerWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class MISSIONSYSTEM_API UMissionTimerWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* TimerBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimerText = nullptr;

public:

	UMissionTimerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	void SetRemainingTime(float RemainingTime, float TotalDuration);

};// end class
