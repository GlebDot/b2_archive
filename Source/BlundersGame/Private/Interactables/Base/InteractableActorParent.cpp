// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Interactables/Base/InteractableActorParent.h"

// Sets default values
AInteractableActorParent::AInteractableActorParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("BlockAll");

	if (Mesh)
	{
		SetRootComponent(Mesh);
	}
}

void AInteractableActorParent::Interact_Implementation(AActor* InteractedActor)
{
	OnInteract();
}

// Called when the game starts or when spawned
void AInteractableActorParent::BeginPlay()
{
	Super::BeginPlay();
	
}

