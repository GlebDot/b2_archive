// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyArmour/ArmourPlate.h"

#include "Enemies/EnemyArmour/ArmourPlateSimulation.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerBase/ResourcePickUps/PickUpTypes/ArmourPickUp.h"


// Sets default values
AArmourPlate::AArmourPlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ArmourPlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armour Plate Mesh"));
	ArmourPlateMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ArmourPlateMesh->SetCollisionProfileName("EnemyCharacter", true);

	if (ArmourPlateMesh)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "Armour created");
		SetRootComponent(ArmourPlateMesh);
		ArmourPlateMesh->SetCollisionProfileName(FName("EnemyCharacter"), true);
	}

	DamageHandler = CreateDefaultSubobject<USimpleDamageHandlerComponent>(TEXT("Damage Handler"));

	Tags.Add(FName("Enemy"));

}

void AArmourPlate::SetHealth(const float Health)
{
	DamageHandler->SetMaxHealth(Health);
}

void AArmourPlate::Destroyed()
{
	Super::Destroyed();
	if (DamageHandler->IsDead())
	{
		const FVector ImpulseVector = 650.0f * UKismetMathLibrary::RandomUnitVector();
		AArmourPlateSimulation *ArmourPlateSimulation = GetWorld()->SpawnActor<AArmourPlateSimulation>(GetActorLocation(), GetActorRotation());
		if (ArmourPlateSimulation)
		{
			ArmourPlateSimulation->ArmourPlateMesh->SetStaticMesh(ArmourPlateMesh->GetStaticMesh());
			ArmourPlateSimulation->ArmourPlateMesh->AddImpulse(ImpulseVector, NAME_None, true);
		}

		AArmourPickUp *ArmourPickUp = GetWorld()->SpawnActor<AArmourPickUp>(GetActorLocation(), GetActorRotation());
		if (ArmourPickUp)
		{
			ArmourPickUp->PickUpMesh->AddImpulse(ImpulseVector, NAME_None, true);
		}

		UChildActorComponent *ChildActorComponent = Cast<UChildActorComponent>(GetParentComponent());
		if (ChildActorComponent)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "Child Actor Component");
			ChildActorComponent->DestroyComponent();
		}
	}
}

