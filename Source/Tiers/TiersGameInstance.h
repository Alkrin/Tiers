// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TiersGameInstance.generated.h"

/**
 * For all of the data that exists outside of an individual match.
 */
UCLASS()
class TIERS_API UTiersGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName PlayerDisplayName = "Player";
	
};
