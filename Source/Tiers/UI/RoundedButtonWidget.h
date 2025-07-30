// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundedButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIERS_API URoundedButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetIsDisabled, FieldNotify)
  bool bIsDisabled;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetIsDisabled(const bool abIsDisabled);

};
