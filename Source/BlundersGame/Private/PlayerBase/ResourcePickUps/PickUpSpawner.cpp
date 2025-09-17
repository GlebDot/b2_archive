// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/ResourcePickUps/PickUpSpawner.h"


#include "DrawDebugHelpers.h"
#include "PlayerBase/ResourcePickUps/PickUpTypes/GeneralPickUp.h"
#include "PlayerBase/ResourcePickUps/PickUpTypes/ArmourPickUp.h"

// Sets default values
APickUpSpawner::APickUpSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}
}

// Called when the game starts or when spawned
void APickUpSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void APickUpSpawner::SpawnPickUps(const EPickUpType Type)
{
	UClass *PickUpClass = nullptr;
	switch (Type)
	{
		case EPickUpType::General:
			PickUpClass = AGeneralPickUp::StaticClass();
			break;
		case EPickUpType::Armour:
			PickUpClass = AArmourPickUp::StaticClass();
			break;
	}
	FVector ImpulseDirection = GetActorUpVector() + GetActorForwardVector() * 0.5;
	ImpulseDirection.Normalize();
	const float Step = 360.0 / PickUpsAmountToSpawn;
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	for (int i = 0; i < PickUpsAmountToSpawn; i++)
	{
		APlayerResourcePickUpParent *PickUp = GetWorld()->SpawnActor<APlayerResourcePickUpParent>(PickUpClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
		if (PickUp)
		{
			const float Strength = FMath::RandRange(SpawnImpulseStrength - 3000.0, SpawnImpulseStrength);
			PickUp->PickUpMesh->AddImpulse(ImpulseDirection * Strength);
		}

		ImpulseDirection = ImpulseDirection.RotateAngleAxis(Step, GetActorUpVector());
	}
	
	SetLifeSpan(2.0);
}
