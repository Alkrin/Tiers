// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TiersGameMode.generated.h"

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
};
