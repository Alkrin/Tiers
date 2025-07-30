// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "TiersGameInstance.h"
#include "TiersGameState.h"
#include "TiersPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "LobbyPlayerRowWidget.h"
#include "UI/RoundedButtonWidget.h"
#include "UI/TextDisplayWidget.h"

void ULobbyWidget::NativeConstruct()
{
  Super::NativeConstruct();

  ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld()));
  if (!TypedGameState)
  {
    UE_LOG(LogTemp, Error, TEXT("ULobbyWidget Failed to initialize TypedGameState."));
    return;
  }
  if (!IsValid(RowWidgetClass))
  {
    UE_LOG(LogTemp, Error, TEXT("ULobbyWidget does not have a RowWidgetClass set."));
    return;
  }

  // When the player list changes (players join or leave the session), we need to update the displayed player list.
  FScriptDelegate Delegate;
  Delegate.BindUFunction(this, FName("HandlePlayerListChanged"));
  TypedGameState->OnPlayerListChangedDelegate.Add(Delegate);

  GeneratePlayerSlots();
  UpdateLaunchButtonDisablement();
  UpdateStatusText();
}

void ULobbyWidget::HandlePlayerListChanged()
{
  if (GetIsThisTheHost())
  {
    UE_LOG(LogTemp, Warning, TEXT("HOST: LobbyWidget::HandlePlayerListChanged()"));
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("CLIENT: LobbyWidget::HandlePlayerListChanged()"));
  }

  // Trigger updates to the displayed player list.
  for (int i = 0; i < PlayerRowWidgets.Num(); ++i)
  {
    PlayerRowWidgets[i]->SetPlayerId(GetPlayerIdForRow(i));
  }

  UpdateLaunchButtonDisablement();
  UpdateStatusText();
}

bool ULobbyWidget::GetIsThisTheHost() const
{
  if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
  {
    if (ATiersPlayerState* PlayerState = PlayerController->GetPlayerState<ATiersPlayerState>())
    {
      if (ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
      {
        if (TypedGameState->PlayerList.Num() > 0)
        {
          if (TypedGameState->PlayerList[0] == PlayerState->GetPlayerId())
          {
            return true;
          }
        }
      }
    }
  }

  return false;
}

bool ULobbyWidget::GetAreAllPlayerSlotsFilled() const
{
  if (ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
  {
    if (
      TypedGameState->PlayerList.Num() == TypedGameState->NumHumans && 
      // A slot with a zero indicates that a player joined, but has disconnected or logged off.
      TypedGameState->PlayerList.Find(0) == INDEX_NONE)
    {
      return true;
    }
  }

  return false;
}

bool ULobbyWidget::GetAreAllPlayersReady() const
{
  if (ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
  {
    for (APlayerState* PlayerState : TypedGameState->PlayerArray)
    {
      if (ATiersPlayerState* TypedPlayerState = Cast<ATiersPlayerState>(PlayerState))
      {
        // If any single player is not ready, then not all players are ready.
        // We only check PlayerStates that have a matching PlayerList entry because when a player abandons a game or
        // gets disconnected, their PlayerState is retained (to support reconnect), but they are removed from the
        // PlayerList so we can let someone else join if appropriate.
        if (TypedGameState->PlayerList.Find(TypedPlayerState->GetPlayerId()) != INDEX_NONE && !TypedPlayerState->bIsReady)
        {
          return false;
        }
      }
    }
  }

  return true;
}

bool ULobbyWidget::GetIsThisPlayerReady() const
{
  if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
  {
    if (ATiersPlayerState* TypedPlayerState = PlayerController->GetPlayerState<ATiersPlayerState>())
    {
      return TypedPlayerState->bIsReady;
    }
  }
  return false;
}

void ULobbyWidget::GeneratePlayerSlots()
{
  UE_LOG(LogTemp, Warning, TEXT("GeneratePlayerSlots()"));

  if (ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
  {
    UE_LOG(LogTemp, Warning, TEXT("TypedGameState exists"));
    // Make sure we know how many slots to make, and that we have somewhere to put them!
    if (TypedGameState->NumHumans > 0 && !!PlayerList)
    {
      UE_LOG(LogTemp, Warning, TEXT("Rowcount data is available.  Rows: %i, Humans: %i"), TypedGameState->MaxPlayers, TypedGameState->NumHumans);
      // Start from an empty slot list.
      PlayerList->ClearChildren();
      PlayerRowWidgets.Empty();

      for (int i = 0; i < TypedGameState->MaxPlayers; ++i)
      {
        // Make a blank row widget.
        if (IsValid(RowWidgetClass))
        {
          ULobbyPlayerRowWidget* RowWidget = Cast<ULobbyPlayerRowWidget>(CreateWidget(GetWorld(), RowWidgetClass));
          if (IsValid(RowWidget))
          {
            // Initialize the row data.
            RowWidget->SetPlayerId(GetPlayerIdForRow(i));

            // Add the new row to the display.
            PlayerRowWidgets.Add(RowWidget);
            PlayerList->AddChildToVerticalBox(RowWidget);
          }
          else
          {
            UE_LOG(LogTemp, Error, TEXT("Failed to instantiate LobbyPlayerRowWidget for index %i"), i);
            continue;
          }
        }
      }
    }
    else
    {
      UE_LOG(LogTemp, Warning, TEXT("Rowcount data unavailable"));
      // Requirements not available yet.  Try again later.
      FTimerHandle RetryHandle;
      GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &ULobbyWidget::GeneratePlayerSlots, 0.5, false);
    }
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("TypedGameState unavailable"));
    // Requirements not available yet.  Try again later.
    FTimerHandle RetryHandle;
    GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &ULobbyWidget::GeneratePlayerSlots, 0.5, false);
  }
}

int32 ULobbyWidget::GetPlayerIdForRow(int32 RowIndex)
{
  int32 PlayerId = 0;
  if (ATiersGameState* TypedGameState = Cast<ATiersGameState>(UGameplayStatics::GetGameState(GetWorld())))
  {
    if (RowIndex < TypedGameState->NumHumans)
    {
      // Human slot.
      if (RowIndex < TypedGameState->PlayerList.Num())
      {
        // May be zero again anyway if the player in the slot disconnected.
        PlayerId = TypedGameState->PlayerList[RowIndex];
      }
    }
    else
    {
      // AI slots use negative ids starting from -1.
      PlayerId = -(RowIndex - TypedGameState->NumHumans + 1);
    }
  }
  return PlayerId;
}

void ULobbyWidget::UpdateLaunchButtonDisablement()
{
  // Only the host is allowed to launch a match.
  LaunchButton->SetVisibility(GetIsThisTheHost() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

  // If we don't have enough players, or if anyone isn't ready, we can't launch a match.
  LaunchButton->SetIsDisabled(!GetAreAllPlayerSlotsFilled() || !GetAreAllPlayersReady());
}

void ULobbyWidget::UpdateStatusText()
{
  const bool bIsThisTheHost = GetIsThisTheHost();
  const bool bAreAllSlotsFilled = GetAreAllPlayerSlotsFilled();
  const bool bAreAllPlayersReady = GetAreAllPlayersReady();
  const bool bIsThisPlayerReady = GetIsThisPlayerReady();

  StatusText->SetIsError(!bAreAllPlayersReady || !bAreAllSlotsFilled);

  if (!bIsThisPlayerReady)
  {
    StatusText->SetText(FText::FromString(TEXT("When ready, please click the Ready toggle.")));
  }
  else if (!bAreAllSlotsFilled)
  {
    StatusText->SetText(FText::FromString(TEXT("Waiting for players to join...")));
  }
  else if (!bAreAllPlayersReady)
  {
    StatusText->SetText(FText::FromString(TEXT("Other players not ready...")));
  }
  else
  {
    if (bIsThisTheHost)
    {
      StatusText->SetText(FText::FromString(TEXT("Ready to launch!")));
    }
    else
    {
      StatusText->SetText(FText::FromString(TEXT("Waiting for host...")));
    }
  }
}