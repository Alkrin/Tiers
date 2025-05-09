// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersPlayerPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TiersGameMode.h"

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

  // Desired initial position is focused on the bottom-center of the map, but if we can't find it,
  // we'll just start at the center-center.
  FVector StartPos(0, 0, InitialZoomHeight);
  if (ATiersGameMode* GameMode = Cast<ATiersGameMode>(GetWorld()->GetAuthGameMode()))
  {
    // Every map tile is 100x100cm.  To get to the bottom edge, we only need half of that (50).
    // The -5 puts us slightly inside the map bounds (assuming a non-degenerate map size).
    StartPos.Y = -(GameMode->GetMapHeight() - 5) * 50;
    // After that, we have to account for the initial altitude and 30deg camera angle.
    StartPos.Y -= StartPos.Z / FMath::Tan(PI/6);
  }
  SetActorLocation(StartPos);
}

// Called every frame
void ATiersPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  // Pan is continuous when the mouse is in the panning area,
  // so we can't drive it off of mouse input events.
  HandlePan(DeltaTime);
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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATiersPlayerPawn::HandlePan(float DeltaTime)
{
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
        // Pan Left (positive X direction)
        FVector Location = GetActorLocation();
        const float MaxPanX = GameMode->GetMapWidth() * 50;
        Location.X = FMath::Min(MaxPanX, Location.X + PanSpeed * DeltaTime);
        SetActorLocation(Location);
      }
      else if (MouseX >= ViewportWidth - PanAreaSize)
      {
        // Pan Right (negative X direction)
        FVector Location = GetActorLocation();
        const float MinPanX = GameMode->GetMapWidth() * -50;
        Location.X = FMath::Max(MinPanX, Location.X - PanSpeed * DeltaTime);
        SetActorLocation(Location);
      }

      if (MouseY < PanAreaSize)
      {
        // Pan Up (positive Y direction)
        FVector Location = GetActorLocation();

        const float EdgeOffset = Location.Z / FMath::Tan(PI/6);
        const float MaxPanY = GameMode->GetMapHeight() * 50 - EdgeOffset;

        Location.Y = FMath::Min(MaxPanY, Location.Y + PanSpeed * DeltaTime);
        SetActorLocation(Location);
      }
      else if (MouseY >= ViewportHeight - PanAreaSize)
      {
        // Pan Down (negative Y direction)
        FVector Location = GetActorLocation();
        
        const float EdgeOffset = Location.Z / FMath::Tan(PI/6);
        const float MinPanY = GameMode->GetMapHeight() * -50 - EdgeOffset;
        
        Location.Y = FMath::Max(MinPanY, Location.Y - PanSpeed * DeltaTime);
        SetActorLocation(Location);
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
    const float DeltaY = DeltaZ / FMath::Tan(PI/6);
    Location.Y -= DeltaY;

    SetActorLocation(Location);
  }
  else
  {
    // Go up.
    const float NewZ = FMath::Min(MaxZoomHeight, Location.Z + ZoomStep);
    const float DeltaZ = NewZ - Location.Z;
    Location.Z = NewZ;
    // Move backward along the 30deg diagonal so we maintain focal point.
    const float DeltaY = DeltaZ / FMath::Tan(PI/6);
    Location.Y -= DeltaY;

    SetActorLocation(Location);
  }
}
