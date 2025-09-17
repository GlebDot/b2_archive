// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/PathControllers/ArenaBlockingDoor.h"
#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"

// Sets default values
AArenaBlockingDoor::AArenaBlockingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	if (DoorMesh)
	{
		SetRootComponent(DoorMesh);
	}

	DoorMesh->SetCollisionProfileName(FName("BlockAll"));
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AArenaBlockingDoor::Reset()
{
	Super::Reset();

	OnArenaEnd();
}

void AArenaBlockingDoor::BeginPlay()
{
	Super::BeginPlay();
	DoorMesh->SetVisibility(false);

	if (ArenaMasterRef)
	{
		ArenaMasterRef->ArenaStartDelegate.AddUFunction(this, FName("OnArenaStart"));
		ArenaMasterRef->ArenaEndDelegate.AddUFunction(this, FName("OnArenaEnd"));
	}
}

void AArenaBlockingDoor::OnArenaStart() const
{
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DoorMesh->SetVisibility(true);
}

void AArenaBlockingDoor::OnArenaEnd() const
{
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DoorMesh->SetVisibility(false);
}


