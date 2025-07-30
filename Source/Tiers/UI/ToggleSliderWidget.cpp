// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleSliderWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "ToggleSliderDecorationInterface.h"
#include "Animation/WidgetAnimation.h"

void UToggleSliderWidget::NativeConstruct()
{
  Super::NativeConstruct();

  OnIsSelectedChanged.AddDynamic(this, &UToggleSliderWidget::HandleIsSelectedChanged);
  Button->OnClicked.AddDynamic(this, &UToggleSliderWidget::HandleClicked);
}

void UToggleSliderWidget::SetIsSelected(bool IsSelected)
{
  if (bIsSelected != IsSelected)
  {
    bIsSelected = IsSelected;
    OnIsSelectedChanged.Broadcast(ReportedValue, bIsSelected);
  }
}

void UToggleSliderWidget::HandleIsSelectedChanged(const FString& aReportedValue, const bool aIsSelected)
{
  if (IsValid(SelectedAnim))
  {
    PlayAnimation(SelectedAnim, 0.f, 1, bIsSelected ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
  }
}

void UToggleSliderWidget::HandleClicked()
{
  UE_LOG(LogTemp, Warning, TEXT("HandleClicked()"));

  if (bIsSingle)
  {
    SetIsSelected(!bIsSelected);
  }
  OnClicked.Broadcast(ReportedValue, bIsSelected);
}

void UToggleSliderWidget::InitializeProps(
    const bool IsSingle,
    const bool IsSelected,
    const FString& aReportedValue,
    TSubclassOf<UUserWidget> aDecorationWidgetClass,
    FMargin aDecorationPadding)
{
  bIsSingle = IsSingle;
  bIsSelected = IsSelected;
  ReportedValue = aReportedValue;
  DecorationWidgetClass = aDecorationWidgetClass;
  DecorationPadding = aDecorationPadding;
}

void UToggleSliderWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  // For some reason, color change does not work consistently in the editor.  Works fine at runtime.
  // Specifically, it works ONCE when you change IsSelected.  After that you have to hit Compile
  // in order for the change to show up.
  LED->SetColorAndOpacity(bIsSelected ? SelectedColor : UnselectedColor);
  
  Toggle->SetRenderTranslation(FVector2D(0.0, bIsSelected ? -15.0 : 15.0));

  if (DecorationWidget)
  {
    DecorationWidget->RemoveFromParent();
    DecorationWidget = nullptr;
  }

  if (IsValid(DecorationWidgetClass))
  {
    DecorationWidget = CreateWidget(GetWorld(), DecorationWidgetClass);

    if (IsValid(DecorationWidget))
    {
      if (DecorationWidget->Implements<UToggleSliderDecoration>())
      {
        IToggleSliderDecoration::Execute_SetDisplayText(DecorationWidget, ReportedValue);
        IToggleSliderDecoration::Execute_UpdateForSelectionStatus(DecorationWidget, bIsSelected);
      }
      DecorationWidget->SetPadding(DecorationPadding);

      TArray<UWidget*> OldChildren = Root->GetAllChildren();
      Root->ClearChildren();

      Root->AddChild(DecorationWidget);
      for(UWidget* Child : OldChildren)
      {
        Root->AddChild(Child);
      }
    }
  }
}
