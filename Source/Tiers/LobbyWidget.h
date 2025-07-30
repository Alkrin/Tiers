// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UVerticalBox;
class ULobbyPlayerRowWidget;
class URoundedButtonWidget;
class UTextDisplayWidget;

/**
 * 
 */
UCLASS()
class TIERS_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual void NativeConstruct() override;
  
  UFUNCTION()
  void HandlePlayerListChanged();

  UFUNCTION(BlueprintCallable, BlueprintPure)
  bool GetIsThisTheHost() const;

  UFUNCTION(BlueprintCallable, BlueprintPure)
  bool GetAreAllPlayerSlotsFilled() const;

  UFUNCTION(BlueprintCallable, BlueprintPure)
  bool GetAreAllPlayersReady() const;

  UFUNCTION(BlueprintCallable, BlueprintPure)
  bool GetIsThisPlayerReady() const;

  void GeneratePlayerSlots();

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UUserWidget> RowWidgetClass;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  UVerticalBox* PlayerList;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  URoundedButtonWidget* LaunchButton;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
  UTextDisplayWidget* StatusText;

private:
  int32 GetPlayerIdForRow(int32 RowIndex);

  UPROPERTY()
  TArray<ULobbyPlayerRowWidget*> PlayerRowWidgets;

  UFUNCTION()
  void UpdateLaunchButtonDisablement();

  UFUNCTION()
  void UpdateStatusText();
};
