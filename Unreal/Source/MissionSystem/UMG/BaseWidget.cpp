#include "BaseWidget.h"

void UBaseWidget::Show()
{
	SetVisibility(ShowVisibility);
	OnShow();
}

void UBaseWidget::Hide()
{
	OnHide();
	SetVisibility(ESlateVisibility::Hidden);
}

void UBaseWidget::OnShow()
{
}

void UBaseWidget::OnHide()
{
}
