// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/DamageTypes/EnemyDamageType.h"

#include "Enemies/EnemyParent.h"

float UEnemyDamageType::ProcessDamage_Implementation(float BaseDamage, AActor* DamagedActor) const
{
    if (DamagedActor->Tags.Contains("Enemy"))
    {
        return 0.0;
    }

    return BaseDamage;
}
