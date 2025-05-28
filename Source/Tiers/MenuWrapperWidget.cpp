// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWrapperWidget.h"

#include "Components/NamedSlot.h"

void UMenuWrapperWidget::SetContentClass(TSubclassOf<UUserWidget> ContentClass)
{
  if (IsValid(ContentClass))
  {
    UUserWidget* Content = CreateWidget(GetWorld(), ContentClass);

    if (IsValid(Content))
    {
      ContentSlot->ClearChildren();
      ContentSlot->AddChild(Content);
    }
  }
}
