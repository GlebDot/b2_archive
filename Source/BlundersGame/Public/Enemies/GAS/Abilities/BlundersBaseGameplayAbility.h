// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enemies/GAS/GASEnemyBase.h"

#include "BlundersBaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UBlundersBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:

    UPROPERTY(BlueprintReadWrite, Category="AbilitySettings")
    AGASEnemyBase *CurrentEnemy;
};
