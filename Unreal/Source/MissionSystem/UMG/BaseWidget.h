#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

UCLASS(Abstract)
class MISSIONSYSTEM_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	ESlateVisibility ShowVisibility = ESlateVisibility::Visible;

public:

	void Show();
	void Hide();

protected:

	virtual void OnShow();
	virtual void OnHide();

};// end class
