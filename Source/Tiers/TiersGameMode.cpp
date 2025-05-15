// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersGameMode.h"

#include "TiersBuilding.h"
#include "TiersRobotCharacter.h"
#include "TiersPlayerController.h"
#include "TiersPlayerPawn.h"
#include "Kismet/GameplayStatics.h"

void ATiersGameMode::PrepareBoard(AActor* BoardActor)
{
  if (!BoardActor)
  {
    UE_LOG(LogTemp, Fatal, TEXT("No board actor found.  Unable to initialize game."));
  }

  ResizeBoard(BoardActor);
  SetUpTeams();
}

void ATiersGameMode::ResizeBoard(AActor* BoardActor)
{
  BoardActor->SetActorScale3D(FVector(MapWidth, MapHeight,1.f));
}

void ATiersGameMode::SetUpTeams()
{
  // First, we need to assign the player(s) and AI(s) to teams.
  // TODO: Spawn OverlordAIControllers for non-human teams.
  // TODO: Assign to more than just the local player!
  ATiersPlayerController* PlayerController = Cast<ATiersPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
  if (PlayerController)
  {
    const int32 TeamIndex = FMath::RandRange(0, TeamSpawners.Num() - 1);
    PlayerController->SetTeamIndex(TeamIndex);
    if (ATiersPlayerPawn* PlayerPawn = Cast<ATiersPlayerPawn>(PlayerController->GetPawn()))
    {
      PlayerPawn->SetUpForTeam(TeamSpawners[TeamIndex]);
    }
  }

  // Then we spawn the buildings and robots for all teams.
  for (int32 i = 0; i < TeamSpawners.Num(); ++i)
  {
    FSpawnDef_Team& TeamDef = TeamSpawners[i];

    // Buildings
    SpawnBuilding(i, TeamDef, TeamDef.HQ);
    for (FSpawnDef_Building& BuildingDef : TeamDef.Buildings)
    {
      SpawnBuilding(i, TeamDef, BuildingDef);
    }

    // Robots
    for (FSpawnDef_Robot& RobotDef : TeamDef.Robots)
    {
      SpawnRobot(i, TeamDef, RobotDef);
    }
  }
}

void ATiersGameMode::SpawnBuilding(const int32 TeamIndex, const FSpawnDef_Team& TeamDef, const FSpawnDef_Building& SpawnDef)
{
  if (SpawnDef.BuildingBlueprint)
  {
    FVector2D Pos = (TeamDef.SpawnCenter + SpawnDef.PositionOffset) * 100.f;
    FVector Location(Pos, 0.f);
    ATiersBuilding* NewBuilding = GetWorld()->SpawnActor<ATiersBuilding>(
      SpawnDef.BuildingBlueprint,
      Location,
      TeamDef.SpawnDirection);
    NewBuilding->SetTeamIndex(TeamIndex);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Missing BuildingBlueprint!"));
  }
}

void ATiersGameMode::SpawnRobot(const int32 TeamIndex, const FSpawnDef_Team& TeamDef, const FSpawnDef_Robot& SpawnDef)
{
  if (SpawnDef.RobotBlueprint)
  {
    FVector2D Pos = (TeamDef.SpawnCenter + SpawnDef.PositionOffset) * 100.f;
    FVector Location(Pos, 50.f);
    ATiersRobotCharacter* NewRobot = GetWorld()->SpawnActor<ATiersRobotCharacter>(
      SpawnDef.RobotBlueprint,
      Location,
      TeamDef.SpawnDirection);
   NewRobot->SetTeamIndex(TeamIndex);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Missing BuildingBlueprint!"));
  }
}
