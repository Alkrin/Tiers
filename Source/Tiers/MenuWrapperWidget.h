// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuTransitionEnum.h"
#include "MenuWrapperWidget.generated.h"

class UCanvasPanel;

/**
 * 
 */
UCLASS()
class TIERS_API UMenuWrapperWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintImplementableEvent)
  void PerformTransitionAnimation(bool IsForward);

  void SetTransition(EMenuTransitionEnum NewTransition) { Transition = NewTransition; };
  UUserWidget* GenerateContent(TSubclassOf<UUserWidget> ContentClass);

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
  UNamedSlot* ContentSlot;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  EMenuTransitionEnum Transition = EMenuTransitionEnum::None;
  
private:

};
