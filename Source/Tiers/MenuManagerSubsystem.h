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
  void ConnectBlueprints(
    TSubclassOf<UUserWidget> MenuWrapperClass,
    TSubclassOf<UUserWidget> DialogWrapperClass,
    TSubclassOf<UUserWidget> InfoDialogClass);

  UFUNCTION(BlueprintCallable)
  void PushMenuWidget(TSubclassOf<UUserWidget> MenuClass, EMenuTransitionEnum Transition = EMenuTransitionEnum::None);

  UFUNCTION(BlueprintCallable)
  void PopMenuWidget(bool ShouldAnimate);

  UFUNCTION(BlueprintCallable)
  void PopMenuWidgetWithTransition(EMenuTransitionEnum Transition);

  UFUNCTION(BlueprintCallable)
  void ClearAllMenus(bool ShouldAnimate);

  UFUNCTION(BlueprintCallable)
  void PushDialogWidget(TSubclassOf<UUserWidget> DialogClass);

  UFUNCTION(BlueprintCallable)
  void PushInfoDialog(const FText Title, const FText Message, bool bIsError = false);

  UFUNCTION(BlueprintCallable)
  void PopDialogWidget(bool ShouldAnimate);

  UFUNCTION(BlueprintCallable)
  void ClearAllDialogs(bool ShouldAnimate);

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSubclassOf<UUserWidget> MenuWrapperClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSubclassOf<UUserWidget> DialogWrapperClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSubclassOf<UUserWidget> InfoDialogClass;

private:
  UPROPERTY()
  TArray<UMenuWrapperWidget*> MenuStack;

  UPROPERTY()
  TArray<UMenuWrapperWidget*> DialogStack;
	
};
