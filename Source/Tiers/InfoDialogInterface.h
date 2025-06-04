// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InfoDialogInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInfoDialog : public UInterface
{
  GENERATED_BODY()
};

class IInfoDialog
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintImplementableEvent)
  void SetDisplayData(bool bIsError, const FText& Title, const FText& Message);

};