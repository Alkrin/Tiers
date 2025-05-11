// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TiersGameMode.generated.h"

class ATiersBuilding;
class ATiersRobotCharacter;

/**
 * 
 */
UCLASS()
class TIERS_API ATiersGameMode : public AGameModeBase
{
  GENERATED_BODY()
  
public:
  int32 GetMapWidth() const { return MapWidth; }
  int32 GetMapHeight() const { return MapHeight; }

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  int32 MapWidth = 50;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  int32 MapHeight = 50;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> HQ;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> CrystalMine;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> Factory;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> Lab;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Units")
  TSubclassOf<ATiersRobotCharacter> StarterRobot;
};
