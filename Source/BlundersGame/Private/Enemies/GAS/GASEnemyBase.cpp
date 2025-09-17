// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/GAS/GASEnemyBase.h"
#include "AbilitySystemComponent.h"
#include "BlundersGame/Public/DamageComponents/DamageHandlerComponent.h"
#include "BlundersGame/Public/EnemyAI/EnemyParentAIController.h"

AGASEnemyBase::AGASEnemyBase()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(FName("AbilitySystemComponent"));
}

void AGASEnemyBase::BeginPlay()
{
    Super::BeginPlay();


    for (auto &Ability : Abilities)
    {
        Ability.Value.AbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.Value.Ability, 1, static_cast<int32>(Ability.Key)));
    }

    if (AbilitySystemComponent->AbilityActorInfo)
    {
        AbilitySystemComponent->AbilityActorInfo->AnimInstance = GetMesh()->GetAnimInstance();
        AbilitySystemComponent->AbilityActorInfo->OwnerActor = this;
    }
}

void AGASEnemyBase::Reset()
{
    AbilitySystemComponent->TargetCancel();
    
    Super::Reset();
}

UAbilitySystemComponent* AGASEnemyBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AGASEnemyBase::ActivateAbility(const EEnemyAbilityType AbilityType)
{
    if (Abilities.Contains(AbilityType))
    {
        const FGameplayAbilitySpecHandle AbilitySpecHandle = Abilities.Find(AbilityType)->AbilityHandle;
        
        if (AbilitySpecHandle.IsValid() && AbilitySystemComponent)
        {
            EnemyAttacksData->CurrentActiveAbilityType = AbilityType;
            AbilitySystemComponent->TryActivateAbility(AbilitySpecHandle);
        } 
    }
}

void AGASEnemyBase::OnStopEffect(float StunTime)
{
    Super::OnStopEffect(StunTime);
    
    ActivateAbility(EEnemyAbilityType::Stun);
}
