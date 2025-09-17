// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/PlayerSpeedEnergyComponent.h"
#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerAmmoComponent.h"

// Sets default values for this component's properties
UPlayerSpeedEnergyComponent::UPlayerSpeedEnergyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SpeedEnergyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpeedEnergyMesh"));

	const ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Player/SuitMeshes/Meshes/PlaceholderEnergy.PlaceholderEnergy'"));
	if (Mesh.Succeeded())
	{
		SpeedEnergyMesh->SetStaticMesh(Mesh.Object);
	}
}


// Called when the game starts
void UPlayerSpeedEnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Player->MovementManager)
	{
		MaxMovementSpeed = Player->MovementManager->MovementParams->SpeedParams.MovementSpeed;
	}
	
}


// Called every frame
void UPlayerSpeedEnergyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Player && CurrentEnergyCells == 0)
	{
		if (Player->PlayerMovementState == PlayerMovementStates::Run || Player->PlayerMovementState == PlayerMovementStates::Slide)
		{
			CurrentEnergyPerCell += DeltaTime * EnergyAccumulationSpeed * Player->GetVelocity().Size() / MaxMovementSpeed;
			CurrentEnergyPerCell = FMath::Clamp(CurrentEnergyPerCell, 0.0f, MaxEnergyPerCell);
			SpeedEnergyMesh->SetScalarParameterValueOnMaterials(FName("SpeedEnergyPercent"), CurrentEnergyPerCell/MaxEnergyPerCell);
			if (CurrentEnergyPerCell == MaxEnergyPerCell)
			{
				CurrentEnergyPerCell = 0.0f;
				CurrentEnergyCells++;
				SpeedEnergyMesh->SetScalarParameterValueOnMaterials(FName("SpeedEnergyCells"), CurrentEnergyCells);
			}
		}
	}
}

void UPlayerSpeedEnergyComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (SpeedEnergyMesh)
	{
		ACharacter *Character = Cast<ACharacter>(GetOwner());
		SpeedEnergyMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("SpeedEnergySocket"));
	}
}

void UPlayerSpeedEnergyComponent::SetPlayerRef(APlayerCharacter* PlayerCharacter)
{
	Player = PlayerCharacter;
}

void UPlayerSpeedEnergyComponent::UseEnergy(const ESpeedEnergyUsageType UsageType)
{
	if (CurrentEnergyCells > 0 && Player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, "Energy Used");
		CurrentEnergyCells--;
		SpeedEnergyMesh->SetScalarParameterValueOnMaterials(FName("SpeedEnergyCells"), CurrentEnergyCells);
		switch (UsageType)
		{
			case ESpeedEnergyUsageType::Ammo:
				Player->AmmoComponent->AddHeavyAmmo(500);
				Player->AmmoComponent->AddLightAmmo(500);
				break;
			case ESpeedEnergyUsageType::Health:
				Player->DamageHandlerComponent->AddHealthFromPickUp(30);
				break;
		}
	}
}

void UPlayerSpeedEnergyComponent::AddEnergy(const float EnergyAmount)
{
	if (Player && CurrentEnergyCells != MaxEnergyCells)
	{
		CurrentEnergyPerCell += EnergyAmount;
		if (CurrentEnergyPerCell >= MaxEnergyPerCell)
		{
			CurrentEnergyPerCell -= MaxEnergyPerCell;
			CurrentEnergyCells++;
			SpeedEnergyMesh->SetScalarParameterValueOnMaterials(FName("SpeedEnergyCells"), CurrentEnergyCells);
		}

		SpeedEnergyMesh->SetScalarParameterValueOnMaterials(FName("SpeedEnergyPercent"), CurrentEnergyPerCell/MaxEnergyPerCell);
	}
}

