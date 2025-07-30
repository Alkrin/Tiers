// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersGameState.h"
#include "Net/UnrealNetwork.h"
#include "TiersPlayerState.h"
#include "Kismet/GameplayStatics.h"

void ATiersGameState::Initialize(int32 aMaxPlayers, int32 aNumHumans)
{
  MaxPlayers = aMaxPlayers;
  NumHumans = aNumHumans;
}

void ATiersGameState::HandlePlayerJoined(int32 PlayerId)
{
  // Check if there is already an open slot in the array.
  const int32 AvailableSlotIndex = PlayerList.Find(0);
  if (AvailableSlotIndex == INDEX_NONE)
  {
    // There was no available slot, so push to a new slot at the back.
    PlayerList.Add(PlayerId);
  }
  else
  {
    // There was an available slot, so fill it.
    PlayerList[AvailableSlotIndex] = PlayerId;
  }

  if (ATiersPlayerState* PlayerState = Cast<ATiersPlayerState>(UGameplayStatics::GetPlayerState(this, 0)))
  {
    if (PlayerState->GetPlayerId() == PlayerId)
    {
      // When the host "joins", set them as Ready.
      // No need to register for an "isReadyChanged" event for the host, since they are always considered ready.
      PlayerState->bIsReady = true;
    }
  }

  // Calls the update function for the listen server.
  OnRep_PlayerListChanged();
}

void ATiersGameState::HandlePlayerLeft(int32 PlayerId)
{
  // Remove the player, but maintain the empty slot.
  const int32 SlotIndex = PlayerList.Find(PlayerId);
  if (SlotIndex != INDEX_NONE)
  {
    PlayerList[SlotIndex] = 0;

    // When a player goes offline, mark them as unready.
    if (ATiersPlayerState* TypedPlayerState = GetPlayerStateForId(PlayerId))
    {
      UE_LOG(LogTemp, Warning, TEXT("Unreadying disconnected PlayerId %i"), PlayerId);
      TypedPlayerState->bIsReady = false;
    }

    // Calls the update function for the listen server.
    OnRep_PlayerListChanged();
  }
}

void ATiersGameState::OnRep_PlayerListChanged()
{
  // When a change occurs, notify anyone who signed up to be told.
  OnPlayerListChangedDelegate.Broadcast();
}

void ATiersGameState::OnPlayerReadinessChanged()
{
  // Use the same event for player join/leave and ready/unready.
  OnPlayerListChangedDelegate.Broadcast();
}

void ATiersGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ATiersGameState, PlayerList);
  DOREPLIFETIME(ATiersGameState, MaxPlayers);
  DOREPLIFETIME(ATiersGameState, NumHumans);
}

ATiersPlayerState* ATiersGameState::GetPlayerStateForId(int32 PlayerId) const
{
  const TObjectPtr<APlayerState> * PrePlayerState = PlayerArray.FindByPredicate([&](APlayerState* PlayerState)
    {
      return PlayerState->GetPlayerId() == PlayerId;
    });

  if (PrePlayerState)
  {
    return Cast<ATiersPlayerState>(*PrePlayerState);
  }
  else
  {
    return nullptr;
  }
}
