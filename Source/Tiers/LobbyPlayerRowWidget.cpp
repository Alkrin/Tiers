// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerRowWidget.h"

#include "UI/ToggleSliderWidget.h"
#include "UI/TextDisplayWidget.h"
#include "UI/RecessedSwitcherWidget.h"
#include "Components/Image.h"
#include "TiersGameState.h"
#include "TiersPlayerState.h"
#include "Kismet/GameplayStatics.h"

void ULobbyPlayerRowWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // When the ready toggle is clicked, we need to update ready status for all to see.
  FScriptDelegate Delegate;
  Delegate.BindUFunction(this, FName("HandleReadyToggleClicked"));
  ReadyToggle->OnClicked.Add(Delegate);
}

void ULobbyPlayerRowWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  UpdateDisplay();
}

void ULobbyPlayerRowWidget::SetPlayerId(int32 NewPlayerId)
{
  if (PlayerId != NewPlayerId)
  {
    PlayerId = NewPlayerId;
  }

  // Always update the display, since we want to capture any PlayerState updates as well.
  UpdateDisplay();
}

void ULobbyPlayerRowWidget::UpdateDisplay()
{
  if (PlayerId == 0)
  {
    SetUpAsEmptyHumanSlot();
  }
  else if (PlayerId > 0)
  {
    SetUpAsFilledHumanSlot();
  }
  else
  {
    SetUpAsBotSlot();
  }
}

void ULobbyPlayerRowWidget::SetUpAsBotSlot()
{
  // The local player is not a bot.
  SelfIndicator->SetVisibility(ESlateVisibility::Hidden);
  
  // A bot is always ready, so they don't need a toggle.
  ReadyToggle->SetVisibility(ESlateVisibility::Hidden);
  // And they show the "ready" icon instead.
  StatusSwitcher->SetPanelIndex(1);

  PlayerNameTextDisplay->SetIsError(false);
  PlayerNameTextDisplay->SetText(FText::FromString(FString::Printf(TEXT("Bot #%i"), -PlayerId)));
}

void ULobbyPlayerRowWidget::SetUpAsEmptyHumanSlot()
{
  // No player, so it cannot be the local player.
  SelfIndicator->SetVisibility(ESlateVisibility::Hidden);

  // No player, so they don't need a toggle.
  ReadyToggle->SetVisibility(ESlateVisibility::Hidden);
  // But we are waiting someone to join, so show the "waiting" icon.
  StatusSwitcher->SetPanelIndex(0);

  PlayerNameTextDisplay->SetIsError(true);
  PlayerNameTextDisplay->SetText(FText::FromString(TEXT("...")));
}

void ULobbyPlayerRowWidget::SetUpAsFilledHumanSlot()
{
  // When a new player joins a game, they don't have valid PlayerStates right away, so we have to wait for the data
  // to arrive from the server.
  ATiersPlayerState* PlayerState = GetMatchingPlayerState();
  if (!PlayerState)
  {
    FTimerHandle RetryHandle;
    GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &ULobbyPlayerRowWidget::UpdateDisplay, 0.5, false);
    return;
  }

  if (ATiersPlayerState* SelfState = UGameplayStatics::GetPlayerController(this, 0)->GetPlayerState<ATiersPlayerState>())
  {
    const bool bIsSelf = SelfState->GetPlayerId() == PlayerId;
    const bool bIsHost = GetHostPlayerId() == PlayerId;

    SelfIndicator->SetVisibility(bIsSelf ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    // Host always counts as ready.  All other players need a toggle for themself.
    ReadyToggle->SetVisibility(bIsSelf && !bIsHost ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    ReadyToggle->SetIsSelected(PlayerState->bIsReady);
    
    if (bIsHost)
    {
      // The host always shows "ready".
      StatusSwitcher->SetVisibility(ESlateVisibility::Visible);
      StatusSwitcher->SetPanelIndex(1);
    }
    else
    {
      StatusSwitcher->SetVisibility(bIsSelf ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
      StatusSwitcher->SetPanelIndex(PlayerState->bIsReady ? 1 : 0);
    }

    PlayerNameTextDisplay->SetIsError(false);
    PlayerNameTextDisplay->SetText(FText::FromString(PlayerState->GetPlayerName()));
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("PlayerState for PlayerId %i was unavailable in ULobbyPlayerRowWidget."), PlayerId);
    SetUpAsEmptyHumanSlot();
  }
}

void ULobbyPlayerRowWidget::HandleReadyToggleClicked(const FString& ReportedValue, bool bIsSelected)
{
  if (ATiersPlayerState* PlayerState = GetMatchingPlayerState())
  {
    // Propagate the ready status change to all connected clients.
    PlayerState->Server_UpdateIsReady(bIsSelected);
  }
}

ATiersPlayerState* ULobbyPlayerRowWidget::GetMatchingPlayerState()
{
  if (ATiersGameState* GameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
  {
    for (APlayerState* PlayerState : GameState->PlayerArray)
    {
      if (PlayerState->GetPlayerId() == PlayerId)
      {
        return Cast<ATiersPlayerState>(PlayerState);
      }
    }
  }

  return nullptr;
}

int32 ULobbyPlayerRowWidget::GetHostPlayerId()
{
  if (ATiersGameState* GameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
  {
    return GameState->PlayerList[0];
  }

  return 0;
}