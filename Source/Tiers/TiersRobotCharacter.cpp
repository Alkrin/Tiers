// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersRobotCharacter.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ATiersRobotCharacter::ATiersRobotCharacter()
{
   // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectorMesh"));
  SelectorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATiersRobotCharacter::BeginPlay()
{
  Super::BeginPlay();
  
  SelectorMesh->SetVisibility(false);
}

// Called every frame
void ATiersRobotCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

void ATiersRobotCharacter::SetIsSelected_Implementation(bool NewIsSelected)
{
  IsSelected = NewIsSelected;
  SelectorMesh->SetVisibility(IsSelected);
}