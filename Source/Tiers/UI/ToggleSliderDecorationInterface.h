// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToggleSliderDecorationInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UToggleSliderDecoration : public UInterface
{
  GENERATED_BODY()
};

class IToggleSliderDecoration
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintImplementableEvent)
  void SetDisplayText(const FString& Text);

  UFUNCTION(BlueprintImplementableEvent)
  void UpdateForSelectionStatus(const bool bIsSelected);

};