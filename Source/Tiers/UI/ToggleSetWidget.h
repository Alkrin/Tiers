// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToggleSetWidget.generated.h"

class UHorizontalBox;
class UToggleSliderWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleSetSelectionChangedEvent, const FString&, Selection);

/**
 * 
 */
UCLASS()
class TIERS_API UToggleSetWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual void SynchronizeProperties() override;
  virtual void NativeConstruct() override;

  /** Called when CurrentSelection changes value. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FOnToggleSetSelectionChangedEvent OnSelectionChanged;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<FString> Labels;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetCurrentSelection)
  FString CurrentSelection;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetCurrentSelection(const FString& Selection);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  TSubclassOf<UUserWidget> ToggleSliderWidgetClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  TSubclassOf<UUserWidget> DecorationWidgetClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  FMargin DecorationPadding;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  float TogglePadding;

protected:
  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UHorizontalBox* Root;

  UPROPERTY(BlueprintReadWrite)
  TArray<UToggleSliderWidget *> Toggles;

private:
  UFUNCTION()
  void HandleToggleClicked(const FString& ReportedValue, const bool IsSelected);

};
