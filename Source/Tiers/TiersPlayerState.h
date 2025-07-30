// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TiersPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIsReadyChangedDelegate);

/**
 * 
 */
UCLASS()
class TIERS_API ATiersPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
  UPROPERTY(ReplicatedUsing = OnRep_IsReadyChanged, BlueprintReadWrite)
  bool bIsReady;
	
  UFUNCTION()
  virtual void OnRep_IsReadyChanged();

  UPROPERTY(BlueprintAssignable)
	FIsReadyChangedDelegate OnIsReadyChangedDelegate;

  UFUNCTION(Server, Reliable, BlueprintCallable)
  void Server_UpdateIsReady(bool abIsReady);

protected:
  virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
  
};
