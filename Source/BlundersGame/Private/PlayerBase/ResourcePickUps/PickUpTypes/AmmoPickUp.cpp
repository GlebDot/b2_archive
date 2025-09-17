// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/ResourcePickUps/PickUpTypes/AmmoPickUp.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerAmmoComponent.h"

void AAmmoPickUp::TransferResources(APlayerCharacter* Player)
{
    Player->AmmoComponent->AddHeavyAmmo(AmmoToRestore);
    Player->AmmoComponent->AddLightAmmo(AmmoToRestore);
}
