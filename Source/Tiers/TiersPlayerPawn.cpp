// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersPlayerPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TiersGameMode.h"
#include "TiersPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATiersPlayerPawn::ATiersPlayerPawn()
{
   // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATiersPlayerPawn::BeginPlay()
{
  Super::BeginPlay();

}

// Called every frame
void ATiersPlayerPawn::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  // Pan is continuous when the mouse is in the panning area,
  // so we can't drive it off of mouse input events.
  HandlePan(DeltaTime);

  UpdateDragSelection();
}

void ATiersPlayerPawn::NotifyControllerChanged()
{
  Super::NotifyControllerChanged();

  // Add Input Mapping Context
  if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
  {
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
      Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
  }
}

// Called to bind functionality to input
void ATiersPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  // Set up action bindings
  if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
  {
    // Zoom
    EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATiersPlayerPawn::HandleZoom);
    // Select
    EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ATiersPlayerPawn::HandleSelect);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
  }
}

void ATiersPlayerPawn::HandlePan(float DeltaTime)
{
  // No pan during selection.
  if (IsSelecting)
  {
    return;
  }

  // If mouse is near the edges of the viewport, and the pawn is still in the permitted volume, pan!
  if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
  {
    float MouseX;
    float MouseY;
    if (PlayerController->GetMousePosition(MouseX, MouseY))
    {
      ATiersGameMode* GameMode = Cast<ATiersGameMode>(GetWorld()->GetAuthGameMode());
      if (!GameMode)
      {
        return;
      }

      int32 ViewportWidth;
      int32 ViewportHeight;
      PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

      if (MouseX < PanAreaSize)
      {
        // Pan Left
        FVector RequestedMove = -GetActorRightVector() * PanSpeed * DeltaTime;
        ApplyRequestedMove(RequestedMove);
      }
      else if (MouseX >= ViewportWidth - PanAreaSize)
      {
        // Pan Right
        FVector RequestedMove = GetActorRightVector() * PanSpeed * DeltaTime;
        ApplyRequestedMove(RequestedMove);
      }

      if (MouseY < PanAreaSize)
      {
        // Pan Forward
        FVector RequestedMove = GetActorForwardVector() * PanSpeed * DeltaTime;
        ApplyRequestedMove(RequestedMove);
      }
      else if (MouseY >= ViewportHeight - PanAreaSize)
      {
        // Pan Backward
        FVector RequestedMove = -GetActorForwardVector() * PanSpeed * DeltaTime;
        ApplyRequestedMove(RequestedMove);
      }
    }
  }
}

void ATiersPlayerPawn::HandleZoom(const FInputActionValue& Value)
{
  const float Delta = Value.Get<float>();
  FVector Location = GetActorLocation();
  // If the pawn is still in the permitted volume, zoom by moving along the permitted diagonal (30 degrees).
  if (Delta > 0)
  {
    // Go down.
    const float NewZ = FMath::Max(MinZoomHeight, Location.Z - ZoomStep);
    const float DeltaZ = NewZ - Location.Z;
    Location.Z = NewZ;
    // Move forward along the 30deg diagonal so we maintain focal point.
    const float Magnitude = DeltaZ / FMath::Tan(PI/6);
    FVector DeltaForward = GetActorForwardVector() * Magnitude;
    Location -= DeltaForward;

    SetActorLocation(Location);
  }
  else
  {
    // Go up.
    const float NewZ = FMath::Min(MaxZoomHeight, Location.Z + ZoomStep);
    const float DeltaZ = NewZ - Location.Z;
    Location.Z = NewZ;
    // Move backward along the 30deg diagonal so we maintain focal point.
    const float Magnitude = DeltaZ / FMath::Tan(PI/6);
    FVector DeltaBackward = -GetActorForwardVector() * Magnitude;
    Location += DeltaBackward;

    SetActorLocation(Location);
  }
}

void ATiersPlayerPawn::HandleSelect(const FInputActionValue& Value)
{
  IsSelecting = Value.Get<bool>();

  if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(Controller))
  {
    if (IsSelecting)
    {
      PlayerController->BeginDragSelect();
    }
    else
    {
      PlayerController->EndDragSelect();
    }
  }
}

void ATiersPlayerPawn::UpdateDragSelection()
{
  if (IsSelecting)
  {
    if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(Controller))
    {
      PlayerController->UpdateDragSelection();
    }
  }
}

void ATiersPlayerPawn::SetUpForTeam(const FSpawnDef_Team& TeamDef)
{
  // Set up camera position and controls.
  // Desired initial position is focused on the team's HQ, but if we can't find it,
  // we'll just start at the center of the map.
  FVector StartPos(0, 0, InitialZoomHeight);
  if (ATiersGameMode* GameMode = Cast<ATiersGameMode>(GetWorld()->GetAuthGameMode()))
  {
    StartPos.X = (TeamDef.SpawnCenter.X + TeamDef.HQ.PositionOffset.X) * 100.f;
    StartPos.Y = (TeamDef.SpawnCenter.Y + TeamDef.HQ.PositionOffset.Y) * 100.f;

    // Now, move the camera "back" according to the 30deg camera angle and the spawn direction.
    FVector CameraOffset(0, -StartPos.Z / FMath::Tan(PI/6), 0);
    CameraOffset = TeamDef.SpawnDirection.RotateVector(CameraOffset);
    StartPos += CameraOffset;

    SetActorRotation(UKismetMathLibrary::ComposeRotators(GetActorRotation(), TeamDef.SpawnDirection));
  }
  SetActorLocation(StartPos);
}

void ATiersPlayerPawn::ApplyRequestedMove(FVector Delta)
{
  ATiersGameMode* GameMode = Cast<ATiersGameMode>(GetWorld()->GetAuthGameMode());
  if (!GameMode)
  {
    return;
  }
  const float MapXMax = GameMode->GetMapWidth() / 2.f * 100.f;
  const float MapXMin = -MapXMax;
  const float MapYMax = GameMode->GetMapHeight() / 2.f * 100.f;
  const float MapYMin = -MapYMax;

  FVector NewLocation = GetActorLocation() + Delta;
  // Constrain all moves so the focal point remains within the map boundaries.
  const float EdgeOffset = NewLocation.Z / FMath::Tan(PI/6);
  FVector NewFocalPoint = FVector(NewLocation.X, NewLocation.Y, 0) + GetActorForwardVector() * EdgeOffset;

  const float XOverflow = FMath::Max(0.f, NewFocalPoint.X - MapXMax);
  NewLocation.X -= XOverflow;
  const float XUnderflow = FMath::Min(0.f, NewFocalPoint.X - MapXMin);
  NewLocation.X -= XUnderflow;

  const float YOverflow = FMath::Max(0.f, NewFocalPoint.Y - MapYMax);
  NewLocation.Y -= YOverflow;
  const float YUnderflow = FMath::Min(0.f, NewFocalPoint.Y - MapYMin);
  NewLocation.Y -= YUnderflow;

  SetActorLocation(NewLocation);
}