// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyArmour/ArmourContainerComponent.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UArmourContainerComponent::UArmourContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArmourContainerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (ArmourSettingsList.Num() > ArmourComponents.Num())
	{
		AEnemyParent *Owner = Cast<AEnemyParent>(GetOwner());
		for (auto &Settings : ArmourSettingsList)
		{
		
			UChildActorComponent *Component = NewObject<UChildActorComponent>(Owner, UChildActorComponent::StaticClass());
			Component->SetChildActorClass(AArmourPlate::StaticClass());
			Component->CreateChildActor();
			AArmourPlate *Plate = Cast<AArmourPlate>(Component->GetChildActor());
		
			if (Plate)
			{
				Plate->SetHealth(ArmourHealth);
				Plate->ArmourPlateMesh->SetStaticMesh(Settings.PlateMesh);
				Plate->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Settings.AttachSocketName);
				if (Settings.IsMirrored)
				{
					Plate->ArmourPlateMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, -1.0f));
				}
			}
			
			Owner->AddComponent(FName("Armour"), true, FTransform::Identity, Component);
			ArmourComponents.Add(Component);		
		}
	}
}

void UArmourContainerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	for (auto &ArmourComponent : ArmourComponents)
	{
		if (ArmourComponent)
		{
			ArmourComponent->DestroyChildActor();
			ArmourComponent->DestroyComponent();
		}
	}

	ArmourComponents.Empty();
}

void UArmourContainerComponent::BreakAllArmour()
{
	for (auto &ArmourPlate : ArmourComponents)
	{
		if (ArmourPlate && ArmourPlate->GetChildActor())
		{
			UGameplayStatics::ApplyDamage(ArmourPlate->GetChildActor(), 100000.0f, nullptr, nullptr, UDamageType::StaticClass());
		} 
	}

	ArmourComponents.Empty();
}

float UArmourContainerComponent::DealDamageToArmourPlates(float Damage, AController* EventInstigator, AActor* DamageCauser, const UDamageType *DamageTypeClass)
{
	float EnemyDamage = Damage;
	if (!IsArmourPlatesOn)
	{
		return EnemyDamage;
	}

	IsArmourPlatesOn = false;
	float DamagePlateMultiplier = 1.0f;
	int ActiveArmourPlates = 0;
	const float DamagePlateMultiplierReduction = 1.0f / ArmourSettingsList.Num();

	for (auto &ArmourPlate : ArmourComponents)
	{
		if (ArmourPlate && ArmourPlate->GetChildActor())
		{
			IsArmourPlatesOn = true;
			ActiveArmourPlates++;
			UGameplayStatics::ApplyDamage(ArmourPlate->GetChildActor(), Damage * DamagePlateMultiplier, EventInstigator, DamageCauser, DamageTypeClass->StaticClass());
			DamagePlateMultiplier = FMath::Clamp(DamagePlateMultiplier - DamagePlateMultiplierReduction, 0.0f, DamagePlateMultiplier);
		} 
	}

	if (ActiveArmourPlates == 0)
	{
		EnemyDamage = Damage;
	} else
	{		
		EnemyDamage /= ActiveArmourPlates;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(EnemyDamage));
	
	return EnemyDamage;
}




