// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TiersGameState.generated.h"

class ATiersPlayerController;
class ATiersPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerListChangedDelegate);

/**
 * 
 */
UCLASS()
class TIERS_API ATiersGameState : public AGameState
{
	GENERATED_BODY()

public:
  void Initialize(int32 aMaxPlayers, int32 aNumHumans);

  void HandlePlayerJoined(int32 PlayerId);
  void HandlePlayerLeft(int32 PlayerId);

  // This is used to maintain a consistently-ordered list for display by each client's Lobby UI.
  UPROPERTY(ReplicatedUsing = OnRep_PlayerListChanged, BlueprintReadOnly)
  TArray<int32> PlayerList;

  UFUNCTION()
  virtual void OnRep_PlayerListChanged();

  UFUNCTION()
  virtual void OnPlayerReadinessChanged();

  UPROPERTY(BlueprintAssignable)
	FPlayerListChangedDelegate OnPlayerListChangedDelegate;

  UPROPERTY(Replicated, BlueprintReadOnly)
  int32 MaxPlayers = 0;

  UPROPERTY(Replicated, BlueprintReadOnly)
  int32 NumHumans = 0;

protected:
  virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
  ATiersPlayerState* GetPlayerStateForId(int32 PlayerId) const;  

};
