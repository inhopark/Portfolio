#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class MISSIONSYSTEM_API UHPWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar = nullptr;   // HP 게이지

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText = nullptr;    // "현재HP / 최대HP" 텍스트

	// 게이지가 목표치까지 보간되는 속도 (클수록 빠르게 따라감)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float BarInterpSpeed = 2.0f;

private:

	float TargetPercent = 1.0f;
	float DisplayedPercent = 1.0f;

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	void SetHP(float CurrentHP, float MaxHP);

private:

	UFUNCTION()
	void HandleHPChanged(float CurrentHP, float MaxHP);

};// end class
