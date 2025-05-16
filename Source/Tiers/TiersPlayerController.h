// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TiersPlayerController.generated.h"

class ITiersEntity;
class UDragSelectorWidget;

/**
 * 
 */
UCLASS()
class TIERS_API ATiersPlayerController : public APlayerController
{
  GENERATED_BODY()

public:
  void BeginDragSelect();
  void UpdateDragSelection();
  void EndDragSelect();

  UFUNCTION(BlueprintCallable)
  void SetTeamIndex(int32 NewTeamIndex) { TeamIndex = NewTeamIndex; };

  int32 GetTeamIndex() const { return TeamIndex; }

  TArray<AActor*>& GetSelectedEntities() { return SelectedEntities; }
  
protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Runtime")
  TArray<AActor*> SelectedEntities;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Runtime")
  int32 TeamIndex = 0;

private:
  UPROPERTY(EditDefaultsOnly, Category="UI")
  TSubclassOf<UDragSelectorWidget> DragSelectorWidgetClass;
  UDragSelectorWidget* DragSelectorWidget;

  void InitializeDragSelectorUI();
};
