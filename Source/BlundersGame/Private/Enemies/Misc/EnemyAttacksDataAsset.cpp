// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/Misc/EnemyAttacksDataAsset.h"

FEnemyAbilitySettings &UEnemyAttacksDataAsset::GetCurrentAbilitySettings()
{
    return *EnemyAbilities.Find(CurrentActiveAbilityType);
}
