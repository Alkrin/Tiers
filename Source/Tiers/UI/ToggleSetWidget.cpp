// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleSetWidget.h"

#include "Components/HorizontalBox.h"
#include "ToggleSliderWidget.h"

void UToggleSetWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // If no explicit default selection was specified, set it to the first item.
  if (Labels.Num() > 0 && CurrentSelection == TEXT(""))
  {
    CurrentSelection = Labels[0];
  }
}

void UToggleSetWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  Root->ClearChildren();
  Toggles.Empty();

  if (IsValid(ToggleSliderWidgetClass))
  {
    for (int32 i = 0; i < Labels.Num(); ++i)
    {
      const FString& Label = Labels[i];
      UToggleSliderWidget* Toggle = Cast<UToggleSliderWidget>(CreateWidget(GetWorld(), ToggleSliderWidgetClass));
      Toggle->InitializeProps(false, Label == CurrentSelection, Label, DecorationWidgetClass, DecorationPadding);
      if (i < Labels.Num() - 1)
      {
        Toggle->SetPadding(FMargin(0.0, 0.0, TogglePadding, 0.0));
      }
      Toggle->OnClicked.AddDynamic(this, &UToggleSetWidget::HandleToggleClicked);

      Toggles.Add(Toggle);
      Root->AddChild(Toggle);
    }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("No ToggleSliderWidgetClass was specified for ToggleSetWidget."));
  }
}

void UToggleSetWidget::SetCurrentSelection(const FString& Selection)
{
  if (CurrentSelection != Selection)
  {
    CurrentSelection = Selection;
    OnSelectionChanged.Broadcast(Selection);
  }
}

void UToggleSetWidget::HandleToggleClicked(const FString& ReportedValue, const bool IsSelected)
{
  if (!IsSelected)
  {
    for (UToggleSliderWidget* Toggle : Toggles)
    {
      Toggle->SetIsSelected(Toggle->ReportedValue == ReportedValue);
    }

    // Set this last, since it broadcasts. We want all state to be ready before we tell anyone.
    SetCurrentSelection(ReportedValue);
  }
}