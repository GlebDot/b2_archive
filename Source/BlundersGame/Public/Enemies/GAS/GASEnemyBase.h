// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyParent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "Enemies/Misc/EnemyAttacksDataAsset.h"
#include "GASEnemyBase.generated.h"

USTRUCT()
struct FGameplayAbilityInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly);
    TSubclassOf<UGameplayAbility> Ability;

    FGameplayAbilitySpecHandle AbilityHandle;
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AGASEnemyBase : public AEnemyParent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
    AGASEnemyBase();

    virtual void BeginPlay() override;

    virtual void Reset() override;

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    UFUNCTION()
    void ActivateAbility(const EEnemyAbilityType AbilityType);        

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Enemy|Base")
    UEnemyAttacksDataAsset *EnemyAttacksData;
protected:    
    UPROPERTY()
    UAbilitySystemComponent *AbilitySystemComponent;

    UPROPERTY(EditDefaultsOnly, Category="Enemy|Base")
    TMap<EEnemyAbilityType, FGameplayAbilityInfo> Abilities;

    virtual void OnStopEffect(float StunTime) override;
};
