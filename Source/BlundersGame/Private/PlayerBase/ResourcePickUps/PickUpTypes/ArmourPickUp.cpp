// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/ResourcePickUps/PickUpTypes/ArmourPickUp.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"

void AArmourPickUp::SetArmourAmount(const int Armour)
{
    ArmourAmount = Armour;
}

int AArmourPickUp::GetArmourAmount() const
{
    return ArmourAmount;
}

void AArmourPickUp::TransferResources(APlayerCharacter* Player)
{
    Player->DamageHandlerComponent->AddArmourFromPickUp(ArmourAmount);
}
