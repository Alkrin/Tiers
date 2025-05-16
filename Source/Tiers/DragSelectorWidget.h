// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragSelectorWidget.generated.h"

class ATiersPlayerController;
class UBorder;

/**
 * 
 */
UCLASS()
class TIERS_API UDragSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
  void StartSelection();
  void UpdateSelection();
  void ClearSelection();
  void FinalizeSelection();

protected:
  // This is how many box sweeps to use when approximating the area under a drag select action.
  // Higher numbers are more accurate but more costly.
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 NumSweepSegments = 8;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FVector2D SelectionStart = FVector2D::ZeroVector;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
  UBorder* SelectionBorder;

private:
  bool GetScaledMousePosition(FVector2D& OutPos);
  void HandleSingleSelect(bool IsShift);
  void HandleBoxSelect(bool IsShift);
  void ClearAllSelections(ATiersPlayerController* PlayerController);
  FVector ScreenToWorldPosition(ATiersPlayerController* PlayerController, const FVector2D& ScreenPos) const;
};
