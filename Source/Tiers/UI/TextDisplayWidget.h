// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextDisplayWidget.generated.h"

class UBorder;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TIERS_API UTextDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  virtual void SynchronizeProperties() override;
	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetIsError, meta = (ExposeOnSpawn="true"))
  bool bIsError;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetIsError(const bool NewIsError);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetText, meta = (ExposeOnSpawn="true"))
  FText Text;
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
  void SetText(const FText NewText);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  float FontSize = 28.f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
  TEnumAsByte<ETextJustify::Type> TextAlignment = ETextJustify::Left;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FLinearColor RootColorNormal = FLinearColor(0.f, 0.010417f, 0.000509f, 1.f);

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FLinearColor RootColorError = FLinearColor(0.010417f, 0.f, 0.000509f, 1.f);

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FLinearColor BackgroundColorNormal = FLinearColor(0.f, 0.022916, 0.00066f, 1.f);

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FLinearColor BackgroundColorError = FLinearColor(0.022916, 0.f, 0.00066f, 1.f);

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FSlateColor TextColorNormal = FSlateColor(FLinearColor(0.f, 0.8f, 0.f, 1.f));

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  FSlateColor TextColorError = FSlateColor(FLinearColor(0.8f, 0.f, 0.f, 1.f));

protected:
  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UBorder* RootBorder;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UBorder* BackgroundBorder;

  UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
  UTextBlock* TextBlock;

  UFUNCTION()
  virtual void UpdateColors();
};
