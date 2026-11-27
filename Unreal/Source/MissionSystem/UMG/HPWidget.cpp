#include "HPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/User/MissionSystemCharacter.h"

UHPWidget::UHPWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ShowVisibility = ESlateVisibility::HitTestInvisible;
}

void UHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DisplayedPercent = TargetPercent;
	if(HPBar != nullptr)
	{
		HPBar->SetPercent(DisplayedPercent);
	}
}

void UHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(HPBar != nullptr && FMath::IsNearlyEqual(DisplayedPercent, TargetPercent, KINDA_SMALL_NUMBER) == false)
	{
		DisplayedPercent = FMath::FInterpTo(DisplayedPercent, TargetPercent, InDeltaTime, BarInterpSpeed);
		HPBar->SetPercent(DisplayedPercent);
	}
}

void UHPWidget::NativeDestruct()
{
	UnbindPlayer();

	Super::NativeDestruct();
}

void UHPWidget::OnShow()
{
	Super::OnShow();

	BindPlayer();
}

void UHPWidget::OnHide()
{
	Super::OnHide();

	UnbindPlayer();
}

void UHPWidget::SetHP(float CurrentHP, float MaxHP)
{
	TargetPercent = MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;

	if(HPText != nullptr)
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(CurrentHP), FMath::RoundToInt(MaxHP))));
	}
}

void UHPWidget::HandleHPChanged(float CurrentHP, float MaxHP)
{
	SetHP(CurrentHP, MaxHP);
}

void UHPWidget::BindPlayer()
{
	UnbindPlayer();

	AMissionSystemCharacter* PlayerCharacter = Cast<AMissionSystemCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("## HPWidget ## BindPlayer ## Error : Player Not Found"));
		return;
	}

	BoundPlayer = PlayerCharacter;
	PlayerCharacter->OnHPChanged.AddDynamic(this, &UHPWidget::HandleHPChanged);
	SetHP(PlayerCharacter->GetCurrentHP(), PlayerCharacter->GetMaxHP());

	UE_LOG(LogTemp, Log, TEXT("## HPWidget ## BindPlayer ## Player : %s"), *PlayerCharacter->GetName());
}

void UHPWidget::UnbindPlayer()
{
	AMissionSystemCharacter* PlayerCharacter = BoundPlayer.Get();
	BoundPlayer.Reset();
	if(PlayerCharacter == nullptr)
	{
		return;
	}

	PlayerCharacter->OnHPChanged.RemoveDynamic(this, &UHPWidget::HandleHPChanged);
}
