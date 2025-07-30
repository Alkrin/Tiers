// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToggleSliderWidget.generated.h"

class UBorder;
class UButton;
class UImage;
class UVerticalBox;
class UWidgetAnimation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnToggleClickedEvent, const FString&, ReportedValue, const bool, IsSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIsSelectedChangedEvent, const FString&, ReportedValue, const bool, IsSelected);

/**
 * 
 */
UCLASS()
class TIERS_API UToggleSliderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual void SynchronizeProperties() override;
  virtual void NativeConstruct() override;

  void InitializeProps(
    const bool IsSingle,
    const bool IsSelected,
    const FString& aReportedValue,
    TSubclassOf<UUserWidget> aDecorationWidgetClass,
    FMargin aDecorationPadding);

  /** Called when the toggle is clicked. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FOnToggleClickedEvent OnClicked;

  /** Called when bIsSelected changes value. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FOnIsSelectedChangedEvent OnIsSelectedChanged;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetIsSelected, meta = (ExposeOnSpawn="true"))
  bool bIsSelected = false;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetIsSelected(bool IsSelected);

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn="true"))
  bool bIsSingle = false;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn="true"))
  FString ReportedValue = TEXT("");

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FLinearColor SelectedColor;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FLinearColor UnselectedColor;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  TSubclassOf<UUserWidget> DecorationWidgetClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  FMargin DecorationPadding;

  UPROPERTY(Transient, meta = (BindWidgetAnim))
  UWidgetAnimation* SelectedAnim = nullptr;

protected:
  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UVerticalBox* Root;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UButton* Button;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UBorder* Toggle;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UImage* LED;

  UPROPERTY(BlueprintReadOnly)
  UUserWidget* DecorationWidget;

private:
  UFUNCTION()
  void HandleIsSelectedChanged(const FString& aReportedValue, const bool aIsSelected);

  UFUNCTION()
  void HandleClicked();
};
