// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TiersEntityInterface.h"
#include "TiersRobotCharacter.generated.h"

UCLASS()
class TIERS_API ATiersRobotCharacter : public ACharacter, public ITiersEntity
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ATiersRobotCharacter();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // ITiersEntity
  void SetIsSelected_Implementation(bool NewIsSelected) override;
  bool GetIsSelected_Implementation() const override { return IsSelected; }

  void SetTeamIndex_Implementation(int32 NewTeamIndex) override { TeamIndex = NewTeamIndex; };
  int32 GetTeamIndex_Implementation() const override { return TeamIndex; }

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Runtime")
  bool IsSelected = false;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Runtime")
  int32 TeamIndex = 0;

private:
  UPROPERTY(EditDefaultsOnly)
  UStaticMeshComponent* SelectorMesh;
};
