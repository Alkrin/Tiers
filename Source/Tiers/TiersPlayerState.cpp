// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "TiersGameState.h"

void ATiersPlayerState::OnRep_IsReadyChanged()
{
  OnIsReadyChangedDelegate.Broadcast();

  // Ready-state changes also count as PlayerList changes, so we manually trigger the event.
  if (ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(this)))
  {
    TypedGameState->OnPlayerListChangedDelegate.Broadcast();
  }
}

void ATiersPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ATiersPlayerState, bIsReady);
}

void ATiersPlayerState::Server_UpdateIsReady_Implementation(bool abIsReady)
{
  bIsReady = abIsReady;
  // Manually call the OnRep function so the listen server gets the update event as well.
  OnRep_IsReadyChanged();
}
