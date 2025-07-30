// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TiersGameMode.generated.h"

class ATiersBuilding;
class ATiersRobotCharacter;
class ATiersPlayerController;

USTRUCT(BlueprintType)
struct FSpawnDef_Building
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<ATiersBuilding> BuildingBlueprint;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FVector2D PositionOffset;
};

USTRUCT(BlueprintType)
struct FSpawnDef_Robot
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<ATiersRobotCharacter> RobotBlueprint;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FVector2D PositionOffset;
};

USTRUCT(BlueprintType)
struct FSpawnDef_Team
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FVector2D SpawnCenter;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FRotator SpawnDirection;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FSpawnDef_Building HQ;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TArray<FSpawnDef_Building> Buildings;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TArray<FSpawnDef_Robot> Robots;
};

/**
 * 
 */
UCLASS()
class TIERS_API ATiersGameMode : public AGameMode
{
  GENERATED_BODY()
  
public:
  ATiersGameMode();

  int32 GetMapWidth() const { return MapWidth; }
  int32 GetMapHeight() const { return MapHeight; }

  UFUNCTION(BlueprintCallable)
  void PrepareBoard(AActor* BoardActor);

  virtual void InitGameState() override;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 MaxPlayers = 2;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 MapWidth = 50;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 MapHeight = 50;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TArray<FSpawnDef_Team> TeamSpawners;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> HQ;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> CrystalMine;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> Factory;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Buildings")
  TSubclassOf<ATiersBuilding> Lab;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Units")
  TSubclassOf<ATiersRobotCharacter> StarterRobot;

  virtual void PostLogin(APlayerController* NewPlayer) override;
  virtual void Logout(AController* Exiting) override;

private:
  void ResizeBoard(AActor* BoardActor);
  void SetUpTeams();
  void SpawnBuilding(const int32 TeamIndex, const FSpawnDef_Team& TeamDef, const FSpawnDef_Building& SpawnDef);
  void SpawnRobot(const int32 TeamIndex, const FSpawnDef_Team& TeamDef, const FSpawnDef_Robot& SpawnDef);
};
