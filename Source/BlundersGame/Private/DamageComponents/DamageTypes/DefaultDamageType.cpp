// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/DamageTypes/DefaultDamageType.h"

float UDefaultDamageType::ProcessDamage_Implementation(float BaseDamage, AActor* DamagedActor) const
{
    return BaseDamage;
}
