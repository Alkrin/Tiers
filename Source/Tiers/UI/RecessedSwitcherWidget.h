// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecessedSwitcherWidget.generated.h"

class UOverlay;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class TIERS_API URecessedSwitcherWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual void SynchronizeProperties() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetPanelIndex)
  int32 PanelIndex;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetPanelIndex(const int32 NewPanelIndex);

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  UOverlay* ContentPanel;

  UPROPERTY(Transient, meta = (BindWidgetAnim))
  UWidgetAnimation* SlideAnim = nullptr;
	
};
