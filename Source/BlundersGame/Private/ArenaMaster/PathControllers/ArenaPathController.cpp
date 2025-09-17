// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/PathControllers/ArenaPathController.h"

// Sets default values
AArenaPathController::AArenaPathController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AArenaPathController::BeginPlay()
{
	Super::BeginPlay();

	if (ArenaRef)
	{
		ArenaRef->ArenaEndDelegate.AddUFunction(this, FName("ArenaEnded"));
		ArenaRef->ArenaStartDelegate.AddUFunction(this, FName("ArenaStarted"));
	}
}

