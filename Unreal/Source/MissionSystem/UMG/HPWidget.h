#pragma once

#include "CoreMinimal.h"
#include "UMG/BaseWidget.h"
#include "HPWidget.generated.h"

class UProgressBar;
class UTextBlock;
class AMissionSystemCharacter;

UCLASS()
class MISSIONSYSTEM_API UHPWidget : public UBaseWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float BarInterpSpeed = 2.0f;

private:

	float TargetPercent = 1.0f;
	float DisplayedPercent = 1.0f;

	TWeakObjectPtr<AMissionSystemCharacter> BoundPlayer;

public:

	UHPWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

protected:

	virtual void OnShow() override;
	virtual void OnHide() override;

public:

	void SetHP(float CurrentHP, float MaxHP);

private:

	UFUNCTION()
	void HandleHPChanged(float CurrentHP, float MaxHP);

	void BindPlayer();
	void UnbindPlayer();

};// end class
