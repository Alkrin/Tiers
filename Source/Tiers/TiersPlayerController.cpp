// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "DragSelectorWidget.h"
#include "MenuManagerSubsystem.h"

// Called when the game starts or when spawned
void ATiersPlayerController::BeginPlay()
{
  Super::BeginPlay();

  // Whenever a player joins a match, they start in the "lobby" and sit there until the Host starts the actual game.
  InitializeLobbyUI();

  // Add the DragSelector first, so it renders under the rest of the HUD.
  //InitializeDragSelectorUI();
  UE_LOG(LogTemp, Warning, TEXT("ATiersPlayerController::BeginPlay()"));
}

void ATiersPlayerController::InitializeLobbyUI()
{
  if (UMenuManagerSubsystem* MenuManager = GetLocalPlayer()->GetSubsystem<UMenuManagerSubsystem>())
  {
    MenuManager->PushMenuWidget(LobbyWidgetClass, EMenuTransitionEnum::FadeAndScale);
  }
}

void ATiersPlayerController::InitializeDragSelectorUI()
{
  DragSelectorWidget = Cast<UDragSelectorWidget>(CreateWidget(this, DragSelectorWidgetClass, TEXT("DragSelector")));
  DragSelectorWidget->ClearSelection();
  DragSelectorWidget->AddToViewport();
}

void ATiersPlayerController::BeginDragSelect()
{
  if (DragSelectorWidget)
  {
    DragSelectorWidget->StartSelection();
  }
}

void ATiersPlayerController::EndDragSelect()
{
  if (DragSelectorWidget)
  {
    DragSelectorWidget->FinalizeSelection();
    DragSelectorWidget->ClearSelection();
  }
}

void ATiersPlayerController::UpdateDragSelection()
{
  if (DragSelectorWidget)
  {
    DragSelectorWidget->UpdateSelection();
  } 
}