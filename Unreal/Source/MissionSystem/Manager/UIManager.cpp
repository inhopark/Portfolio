#include "UIManager.h"
#include "Manager/MissionUISettings.h"
#include "UMG/BaseWidget.h"
#include "GameFramework/PlayerController.h"

void UUIManager::Deinitialize()
{
	ClearUI();

	Super::Deinitialize();
}

void UUIManager::InitializeUI(APlayerController* OwningPlayer)
{
	if(OwningPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## UIManager ## InitializeUI ## Error : OwningPlayer Is Null"));
		return;
	}

	ClearUI();

	const UMissionUISettings* Settings = GetDefault<UMissionUISettings>();
	for(const TPair<EUIType, TSoftClassPtr<UBaseWidget>>& Entry : Settings->WidgetClasses)
	{
		AddUI(OwningPlayer, Entry.Key, Entry.Value);
	}

	UE_LOG(LogTemp, Log, TEXT("## UIManager ## InitializeUI ## UICount : %d"), UIMap.Num());
}

void UUIManager::AddUI(APlayerController* OwningPlayer, EUIType UIType, const TSoftClassPtr<UBaseWidget>& WidgetClass)
{
	if(UIMap.Contains(UIType) == true)
	{
		UE_LOG(LogTemp, Error, TEXT("## UIManager ## AddUI ## Error : Duplicate UI ## UIType : %s"), *UEnum::GetValueAsString(UIType));
		return;
	}

	UClass* LoadedClass = WidgetClass.LoadSynchronous();
	if(LoadedClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## UIManager ## AddUI ## Error : Widget Class Load Failed ## UIType : %s ## Path : %s"), *UEnum::GetValueAsString(UIType), *WidgetClass.ToString());
		return;
	}

	UBaseWidget* NewWidget = CreateWidget<UBaseWidget>(OwningPlayer, LoadedClass);
	if(NewWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## UIManager ## AddUI ## Error : Widget Create Failed ## UIType : %s"), *UEnum::GetValueAsString(UIType));
		return;
	}

	NewWidget->AddToViewport(static_cast<int32>(UIType));
	NewWidget->Hide();

	UIMap.Add(UIType, NewWidget);
}

void UUIManager::ClearUI()
{
	for(const TPair<EUIType, TObjectPtr<UBaseWidget>>& Entry : UIMap)
	{
		if(Entry.Value != nullptr)
		{
			Entry.Value->RemoveFromParent();
		}
	}

	UIMap.Empty();
}
