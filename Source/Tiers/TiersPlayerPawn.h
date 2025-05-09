// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TiersPlayerPawn.generated.h"

class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TIERS_API ATiersPlayerPawn : public APawn
{
  GENERATED_BODY()

public:
  // Sets default values for this pawn's properties
  ATiersPlayerPawn();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:  
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  virtual void NotifyControllerChanged() override;

private:
  UPROPERTY(EditDefaultsOnly, Category="Input")
  int32 PanAreaSize = 10;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  float PanSpeed = 1000;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  float ZoomStep = 250;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  float MinZoomHeight = 500;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  float MaxZoomHeight = 3000;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  float InitialZoomHeight = 1500;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  UInputMappingContext* DefaultMappingContext;

  UPROPERTY(EditDefaultsOnly, Category="Input")
  UInputAction* ZoomAction;

  void HandleZoom(const FInputActionValue& Value);
  void HandlePan(float DeltaTime);
};
