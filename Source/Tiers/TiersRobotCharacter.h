// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TiersRobotCharacter.generated.h"

UCLASS()
class TIERS_API ATiersRobotCharacter : public ACharacter
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ATiersRobotCharacter();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
  void SetSelected(bool NewIsSelected);

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Runtime")
  bool IsSelected = false;

private:
  UPROPERTY(EditDefaultsOnly)
  UStaticMeshComponent* SelectorMesh;
};
