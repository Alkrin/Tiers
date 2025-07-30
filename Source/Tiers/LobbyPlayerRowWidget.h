// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerRowWidget.generated.h"

class UTextDisplayWidget;
class UToggleSliderWidget;
class URecessedSwitcherWidget;
class UImage;
class ATiersPlayerState;

/**
 * 
 */
UCLASS()
class TIERS_API ULobbyPlayerRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual void NativeConstruct() override;
  virtual void SynchronizeProperties() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, BlueprintSetter=SetPlayerId, meta = (ExposeOnSpawn="true"))
  int32 PlayerId;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetPlayerId(const int32 NewPlayerId);

  UFUNCTION(BlueprintCallable)
  void UpdateDisplay();

  UFUNCTION()
  void HandleReadyToggleClicked(const FString& ReportedValue, bool bIsSelected);

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  UImage* SelfIndicator;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  URecessedSwitcherWidget* StatusSwitcher;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  UToggleSliderWidget* ReadyToggle;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  UTextDisplayWidget* PlayerNameTextDisplay;

private:
  UFUNCTION()
  void SetUpAsBotSlot();

  UFUNCTION()
  void SetUpAsEmptyHumanSlot();

  UFUNCTION()
  void SetUpAsFilledHumanSlot();

  UFUNCTION()
  ATiersPlayerState* GetMatchingPlayerState();
  UFUNCTION()
  int32 GetHostPlayerId();

};
