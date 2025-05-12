// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragSelectorWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIERS_API UDragSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintImplementableEvent)
  void StartSelection();

  UFUNCTION(BlueprintImplementableEvent)
  void UpdateSelection();

  UFUNCTION(BlueprintImplementableEvent)
  void ClearSelection();

  UFUNCTION(BlueprintImplementableEvent)
  void FinalizeSelection();

protected:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FVector2D SelectionStart = FVector2D::ZeroVector;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FVector2D SelectionEnd = FVector2D::ZeroVector;
	
};
