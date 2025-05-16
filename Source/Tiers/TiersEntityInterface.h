// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TiersEntityInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTiersEntity : public UInterface
{
  GENERATED_BODY()
};

class ITiersEntity
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tiers Entity")
  int32 GetTeamIndex() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tiers Entity")
  void SetTeamIndex(int32 NewTeamIndex);

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tiers Entity")
  bool GetIsSelected() const;

  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Tiers Entity")
  void SetIsSelected(bool NewIsSelected);
};