// Fill out your copyright notice in the Description page of Project Settings.


#include "RecessedSwitcherWidget.h"

#include "Components/Overlay.h"
#include "Animation/WidgetAnimation.h"

void URecessedSwitcherWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  ContentPanel->SetRenderTranslation(FVector2D(PanelIndex == 0 ? 0.0 : -92.0, 0.0));
}

void URecessedSwitcherWidget::SetPanelIndex(int32 NewPanelIndex)
{
  if (PanelIndex != NewPanelIndex)
  {
    PanelIndex = NewPanelIndex;

    if (IsValid(SlideAnim))
    {
      PlayAnimation(SlideAnim, 0.f, 1, PanelIndex == 1 ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
    }
  }
}