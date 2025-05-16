// Fill out your copyright notice in the Description page of Project Settings.


#include "DragSelectorWidget.h"

#include "TiersPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "TiersBuilding.h"
#include "TiersRobotCharacter.h"
#include "Engine/EngineTypes.h"


void UDragSelectorWidget::StartSelection()
{
  if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
  {
    // Record where we started dragging from.
    GetScaledMousePosition(SelectionStart);
  }
}

void UDragSelectorWidget::UpdateSelection()
{
  FVector2D SelectionEnd;
  if (SelectionBorder && GetScaledMousePosition(SelectionEnd))
  {
    // Update the border widget to be drawn between the drag start position and the current mouse position.
    // Have to account for the fact that the end could be anywhere in relation to the start.
    float X = FMath::Min(SelectionStart.X, SelectionEnd.X);
    float Y = FMath::Min(SelectionStart.Y, SelectionEnd.Y);
    float W = FMath::Max(SelectionStart.X, SelectionEnd.X) - X;
    float H = FMath::Max(SelectionStart.Y, SelectionEnd.Y) - Y;
   
    UCanvasPanelSlot* BorderSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectionBorder);
    BorderSlot->SetPosition(FVector2D(X, Y));
    BorderSlot->SetSize(FVector2D(W, H));
  }
}

void UDragSelectorWidget::ClearSelection()
{
  if (SelectionBorder)
  {
    UCanvasPanelSlot* BorderSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectionBorder);
    BorderSlot->SetSize(FVector2D(0.f, 0.f));
  }
}

void UDragSelectorWidget::FinalizeSelection()
{
  if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
  {
    FVector2D SelectionEnd;
    if (GetScaledMousePosition(SelectionEnd))
    {
      const bool IsShift = PlayerController->IsInputKeyDown(EKeys::LeftShift) || PlayerController->IsInputKeyDown(EKeys::RightShift);
      if (UKismetMathLibrary::Distance2D(SelectionEnd, SelectionStart) <= 5.f)
      {
        HandleSingleSelect(IsShift);
      }
      else
      {
        HandleBoxSelect(IsShift);
      }
    }
  }
}

bool UDragSelectorWidget::GetScaledMousePosition(FVector2D& OutPos)
{
  if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
  {
    float MouseX, MouseY;
    if (PlayerController->GetMousePosition(MouseX, MouseY))
    {
      const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
      OutPos.X = MouseX / Scale;
      OutPos.Y = MouseY / Scale;
      return true;
    }
  }
  return false;
}

void UDragSelectorWidget::HandleSingleSelect(bool IsShift)
{
  // Project the mouse cursor down into the 3d world.
  if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
  {
    FVector2D SelectionEnd;
    PlayerController->GetMousePosition(SelectionEnd.X, SelectionEnd.Y);
    FVector MouseProjection, Direction;
    if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, SelectionEnd, MouseProjection, Direction))
    {
      TArray<FHitResult> Hits;
      TArray<AActor*> ToIgnore;
      UKismetSystemLibrary::BoxTraceMulti(
        this,
        MouseProjection,
        MouseProjection + Direction * 100000,
        FVector(25.f),
        Direction.Rotation(),
        // This just gets converted straight back.  Seems dumb, but you do what you have to.
        UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1),
        false,
        ToIgnore,
        EDrawDebugTrace::None,
        Hits,
        true);
      if (Hits.Num() > 0)
      {
        // If shift is not held down, clear all old selections so we can replace them with the new ones.
        if (!IsShift)
        {
          ClearAllSelections(PlayerController);
        }

        TArray<AActor*>& SelectedEntities = PlayerController->GetSelectedEntities();
        const int32 PlayerTeamIndex = PlayerController->GetTeamIndex();
        for (FHitResult& Hit : Hits)
        {
          AActor* HitActor = Hit.GetActor();
          if (HitActor->Implements<UTiersEntity>())
          {
            if (IsShift)
            {
              if (SelectedEntities.Contains(HitActor))
              {
                // Shift was held, and the entity was already Selected, so we must de-select it.
                ITiersEntity::Execute_SetIsSelected(HitActor, false);
                SelectedEntities.Remove(HitActor);
              }
              else
              {
                // Shift was held, and the entity was not yet Selected.
                
                // If no current selection, just add this entity.
                // Only a single enemy can be selected at a time, so if an enemy entity is already selected,
                //   and we are attempting to add a second, do nothing.
                if (SelectedEntities.Num() == 0 ||
                    (ITiersEntity::Execute_GetTeamIndex(SelectedEntities[0]) == PlayerTeamIndex && ITiersEntity::Execute_GetTeamIndex(HitActor) == PlayerTeamIndex))
                {
                  ITiersEntity::Execute_SetIsSelected(HitActor, true);
                  SelectedEntities.Push(HitActor);
                }
              }
            }
            else
            {
              // Shift was not held, so the hit replaces any old selection.
              ITiersEntity::Execute_SetIsSelected(HitActor, true);
              SelectedEntities.Push(HitActor);
            }
          }
          else
          {
            UE_LOG(LogTemp, Error, TEXT("Does Not Implement ITiersEntity!"));
          }
        }
      }
      else
      {
        ClearAllSelections(PlayerController);
      }
    }
  }
}

void UDragSelectorWidget::HandleBoxSelect(bool IsShift)
{
  if (ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
  {
    TArray<AActor*>& SelectedEntities = PlayerController->GetSelectedEntities();
    TArray<AActor*> NewFriendlySelections;
    TArray<AActor*> NewEnemySelections;

    const int32 PlayerTeamIndex = PlayerController->GetTeamIndex();

    // The stored value of SelectionStart was scaled relative to UI space.  This converts it back to viewport space.
    const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
    FVector2D DragStart(SelectionStart.X * Scale, SelectionStart.Y * Scale);

    FVector2D DragEnd;
    PlayerController->GetMousePosition(DragEnd.X, DragEnd.Y);

    float MaxX = FMath::Max(DragStart.X, DragEnd.X);
    float MinX = FMath::Min(DragStart.X, DragEnd.X);
    float MaxY = FMath::Max(DragStart.Y, DragEnd.Y);
    float MinY = FMath::Min(DragStart.Y, DragEnd.Y);

    FVector TopLeft = ScreenToWorldPosition(PlayerController, FVector2D(MinX, MinY));
    FVector TopRight = ScreenToWorldPosition(PlayerController, FVector2D(MaxX, MinY));
    FVector BottomLeft = ScreenToWorldPosition(PlayerController, FVector2D(MinX, MaxY));
    FVector BottomRight = ScreenToWorldPosition(PlayerController, FVector2D(MaxX, MaxY));

    const float SweepPercent = 1.f / NumSweepSegments;

    // Perform multiple box sweeps to approximate the area under the selection indicator.
    // We can't be precise because the angles of the quadrilateral once projected into world
    // space can be nearly anything.
    float LeftHeight = UKismetMathLibrary::Vector_Distance(TopLeft, BottomLeft);
    float RightHeight = UKismetMathLibrary::Vector_Distance(TopRight, BottomRight);
    float SweepHalfHeight = FMath::Max(LeftHeight, RightHeight) / NumSweepSegments / 2;

    for (int32 SweepIndex = 0; SweepIndex < NumSweepSegments; ++SweepIndex)
    {
      float LerpAlpha = SweepPercent * (SweepIndex + 0.5f);
      FVector SweepStart = UKismetMathLibrary::VLerp(TopLeft, BottomLeft, LerpAlpha);
      FVector SweepEnd = UKismetMathLibrary::VLerp(TopRight, BottomRight, LerpAlpha);
      FRotator SweepOrientation = UKismetMathLibrary::FindLookAtRotation(SweepStart, SweepEnd);

      TArray<FHitResult> Hits;
      TArray<AActor*> ToIgnore;
      UKismetSystemLibrary::BoxTraceMulti(
        this,
        SweepStart,
        SweepEnd,
        FVector(10.f, SweepHalfHeight, 100.f),
        SweepOrientation,
        // This just gets converted straight back.  Seems dumb, but you do what you have to.
        UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1),
        false,
        ToIgnore,
        EDrawDebugTrace::None,
        Hits,
        true);
      for (FHitResult& Hit : Hits)
      {
        AActor* HitActor = Hit.GetActor();
        if (HitActor->Implements<UTiersEntity>())
        {
          if (ITiersEntity::Execute_GetTeamIndex(HitActor) == PlayerTeamIndex)
          {
            NewFriendlySelections.AddUnique(HitActor);
          }
          else
          {
            NewEnemySelections.AddUnique(HitActor);
          }
        }
      }
    }

    // Now that all sweeps have completed, we need to combine the old selections and the new selections.

    // If shift was not held down, then we are replacing the old selection entirely.
    if (!IsShift)
    {
      ClearAllSelections(PlayerController);
    }

    if (SelectedEntities.Num() == 0)
    {
      // If there is nothing currently selected, just add the appropriate actor(s).
      // Prioritize friendly over enemy.
      if (NewFriendlySelections.Num() > 0)
      {
        // All friendlies can be added.
        for (AActor* Friend : NewFriendlySelections)
        {
          ITiersEntity::Execute_SetIsSelected(Friend, true);
          SelectedEntities.Push(Friend);
        }
      }
      else if (NewEnemySelections.Num() > 0)
      {
        // Only a single enemy can be selected at a time.
        ITiersEntity::Execute_SetIsSelected(NewEnemySelections[0], true);
        SelectedEntities.Push(NewEnemySelections[0]);
      }
    }
    else // IsShift == true
    {
      // Something is already selected, so decide whether to add, replace, or ignore.
      const bool PrevSelectionWasFriendly = ITiersEntity::Execute_GetTeamIndex(SelectedEntities[0]) == PlayerTeamIndex;

      if (PrevSelectionWasFriendly)
      {
        // Holding shift, so add any friendlies.
        for (AActor* Friend : NewFriendlySelections)
        {
          ITiersEntity::Execute_SetIsSelected(Friend, true);
          SelectedEntities.Push(Friend);
        }
      }
      // ELSE the prev selection already includes an enemy, and you can only have one selected enemy at a time, so do nothing!
    }
  }
}

void UDragSelectorWidget::ClearAllSelections(ATiersPlayerController* PlayerController)
{
  TArray<AActor*>& SelectedEntities = PlayerController->GetSelectedEntities();
  for(AActor* Actor : SelectedEntities)
  {
    if (Actor->Implements<UTiersEntity>())
    {
      ITiersEntity::Execute_SetIsSelected(Actor, false);
    }
  }

  SelectedEntities.Empty();
}

FVector UDragSelectorWidget::ScreenToWorldPosition(ATiersPlayerController* PlayerController, const FVector2D& ScreenPos) const
{
  FVector MousePos, MouseDirection;
  if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, ScreenPos, MousePos, MouseDirection))
  {
    FHitResult Hit;
    TArray<AActor*> ToIgnore;
    if (UKismetSystemLibrary::LineTraceSingle(
      this,
      MousePos,
      MousePos + MouseDirection * 100000,
      // This just gets converted straight back.  Seems dumb, but you do what you have to.
      UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
      false,
      ToIgnore,
      EDrawDebugTrace::None,
      Hit,
      true))
    {
      return Hit.Location;
    }
  }
  // Failure!
  return FVector::ZeroVector;
}