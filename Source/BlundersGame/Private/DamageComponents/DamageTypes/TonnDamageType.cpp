// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/DamageTypes/TonnDamageType.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"

float UTonnDamageType::ProcessDamage_Implementation(float BaseDamage, AActor* DamagedActor) const
{
    APlayerCharacter *Player = Cast<APlayerCharacter>(DamagedActor);
    if (Player)
    {
        UArmourDamageHandlerComponent *DamageHandler = Cast<UArmourDamageHandlerComponent>(Player->GetComponentByClass(UArmourDamageHandlerComponent::StaticClass()));
        if (DamageHandler)
        {
            int ArmourDamage = DamageHandler->GetArmour();
            ArmourDamage = ArmourDamage / 2;
            DamageHandler->ReduceArmour(ArmourDamage);

            return BaseDamage;
        }
    }
    return 0.0;
}
