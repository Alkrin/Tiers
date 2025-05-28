// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MenuTransitionEnum.h"
#include "MenuManagerSubsystem.generated.h"

class UMenuWrapperWidget;

/**
 * 
 */
UCLASS()
class TIERS_API UMenuManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  void SetMenuWrapperClass(TSubclassOf<UUserWidget> WidgetClass);

  UFUNCTION(BlueprintCallable)
  void PushMenuWidget(TSubclassOf<UUserWidget> MenuClass, EMenuTransitionEnum Transition = EMenuTransitionEnum::None);

  UFUNCTION(BlueprintCallable)
  void PopMenuWidget(bool ShouldAnimate);

  UFUNCTION(BlueprintCallable)
  void PopMenuWidgetWithTransition(EMenuTransitionEnum Transition);

  UFUNCTION(BlueprintCallable)
  void ClearAllMenus(bool ShouldAnimate);

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSubclassOf<UUserWidget> MenuWrapperClass;

private:
  UPROPERTY()
  TArray<UMenuWrapperWidget*> MenuStack;
	
};
