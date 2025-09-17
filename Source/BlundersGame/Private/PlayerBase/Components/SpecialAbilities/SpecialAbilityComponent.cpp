// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/SpecialAbilities/SpecialAbilityComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"

// Sets default values for this component's properties
USpecialAbilityComponent::USpecialAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void USpecialAbilityComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (Player)
	{
		Player->OnSpecialAbilityUsed.Unbind();
	}
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void USpecialAbilityComponent::DetachSpecialAbilityComponent()
{
	Player->RemoveOwnedComponent(this);
	if (Player)
	{
		Player->OnSpecialAbilityUsed.Unbind();
		Player = nullptr;
	}

	DestroyComponent();
}

// Called when the game starts
void USpecialAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetOwner());
	if (Player)
	{
		Player->OnSpecialAbilityUsed.BindUFunction(this, FName("SpecialAbilityUsage"));
	}
	
}

