// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersRobotCharacter.h"

// Sets default values
ATiersRobotCharacter::ATiersRobotCharacter()
{
   // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATiersRobotCharacter::BeginPlay()
{
  Super::BeginPlay();
  
}

// Called every frame
void ATiersRobotCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}
