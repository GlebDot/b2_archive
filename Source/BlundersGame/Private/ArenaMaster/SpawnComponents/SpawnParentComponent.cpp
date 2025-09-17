// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/SpawnComponents/SpawnParentComponent.h"

// Sets default values for this component's properties
USpawnParentComponent::USpawnParentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USpawnParentComponent::SpawnEnd()
{
	if (ArenaRef)
	{
		ArenaRef->SpawnEnd();
	}
}


