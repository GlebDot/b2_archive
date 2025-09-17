// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemyArmour/ArmourPlateSimulation.h"


// Sets default values
AArmourPlateSimulation::AArmourPlateSimulation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ArmourPlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmourPlateMesh"));
	ArmourPlateMesh->SetCollisionProfileName(FName("EnemyArmourSimulation"), true);
	ArmourPlateMesh->SetSimulatePhysics(true);
	if (ArmourPlateMesh)
	{
		SetRootComponent(ArmourPlateMesh);
	}
}

// Called when the game starts or when spawned
void AArmourPlateSimulation::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(10.0);
}

void AArmourPlateSimulation::Reset()
{
	Super::Reset();
	Destroy();
}

