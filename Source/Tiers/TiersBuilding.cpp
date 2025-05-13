// Fill out your copyright notice in the Description page of Project Settings.


#include "TiersBuilding.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATiersBuilding::ATiersBuilding()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

  Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
  // Should receive trace responses for our custom "Selectable" channel.
  Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
  // Should block robots from walking onto it.
  Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
  Collision->SetupAttachment(RootComponent);

  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  Mesh->SetupAttachment(RootComponent);

  SelectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectorMesh"));
  SelectorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATiersBuilding::BeginPlay()
{
  Super::BeginPlay();
  
  SelectorMesh->SetVisibility(false);
}

// Called every frame
void ATiersBuilding::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

void ATiersBuilding::SetSelected(bool NewIsSelected)
{
  IsSelected = NewIsSelected;
  SelectorMesh->SetVisibility(IsSelected);
}