// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/GAS/Abilities/BlundersBaseGameplayAbility.h"

void UBlundersBaseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if(CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        CurrentEnemy = Cast<AGASEnemyBase>(ActorInfo->OwnerActor);
    }

    if (bHasBlueprintActivate)
    {
        K2_ActivateAbility();
    }
}
