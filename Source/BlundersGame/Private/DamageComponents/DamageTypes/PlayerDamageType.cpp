// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/DamageTypes/PlayerDamageType.h"

#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"

float UPlayerDamageType::ProcessDamage_Implementation(float BaseDamage, AActor* DamagedActor) const
{
    APlayerCharacter *Player = Cast<APlayerCharacter>(DamagedActor);

    if (Player)
    {
        return 0.0;
    }

    return BaseDamage;
}
