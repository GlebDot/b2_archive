// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/DamageTypes/EnemyShakerDamageType.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"

float UEnemyShakerDamageType::ProcessDamage_Implementation(float BaseDamage, AActor* DamagedActor) const
{
    if (DamagedActor->Tags.Contains("Enemy"))
    {
        return 0.0;
    }

    ACharacter *Character = Cast<ACharacter>(DamagedActor);
    if (Character)
    {
        FVector LaunchVelocity = FVector::ZeroVector;
        LaunchVelocity.Z = FMath::Clamp(Character->GetVelocity().Size(), 550.0f, 1000.0f);
        Character->LaunchCharacter(LaunchVelocity, true, true);
    }

    return BaseDamage;
}
