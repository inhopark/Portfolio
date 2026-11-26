#include "HPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/User/MissionSystemCharacter.h"

UHPWidget::UHPWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 버튼이 없는 순수 표시용 위젯들이므로 HitTestInvisible로 노출해
	// 다른 위젯(미션 결과창 등)의 클릭을 가로채지 않도록 함
	ShowVisibility = ESlateVisibility::HitTestInvisible;
}

void UHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯이 새로 생성된 시점엔 보간 없이 바로 현재 값으로 표시
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
	// 게이지는 목표치만 갱신 -> NativeTick에서 서서히 보간
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
