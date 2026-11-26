#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Define/UIDefine.h"
#include "UIManager.generated.h"

class UBaseWidget;

UCLASS()
class MISSIONSYSTEM_API UUIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<EUIType, TObjectPtr<UBaseWidget>> UIMap;

public:

	virtual void Deinitialize() override;

public:

	void InitializeUI(APlayerController* OwningPlayer);

	template <typename TWidgetClass>
	TWidgetClass* GetUI(EUIType UIType) const;

private:

	void AddUI(APlayerController* OwningPlayer, EUIType UIType, const TSoftClassPtr<UBaseWidget>& WidgetClass);

	void ClearUI();

};// end class

template <typename TWidgetClass>
TWidgetClass* UUIManager::GetUI(EUIType UIType) const
{
	const TObjectPtr<UBaseWidget>* Widget = UIMap.Find(UIType);
	if(Widget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## UIManager ## GetUI ## Error : UI Not Found ## UIType : %s"), *UEnum::GetValueAsString(UIType));
		return nullptr;
	}

	TWidgetClass* CastedWidget = Cast<TWidgetClass>(Widget->Get());
	if(CastedWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## UIManager ## GetUI ## Error : Type Mismatch ## UIType : %s"), *UEnum::GetValueAsString(UIType));
		return nullptr;
	}

	return CastedWidget;
}
