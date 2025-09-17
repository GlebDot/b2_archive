// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/StatusEffects/AcidStatusEffect.h"
#include "DamageComponents/Damagable.h"
#include "BlundersGame/Public/Enemies/EnemyBaseCharacter.h"
#include "DamageComponents/DamageTypes.h"
#include "Kismet/GameplayStatics.h"

UAcidStatusEffect::UAcidStatusEffect()
{
    StatusEffectType = EStatusEffectType::Acid;
    StatusEffectColor = FVector4(0.0f, 1.0f, 0.7f, 1.0f);
    AcidMaxColor = FVector4(0.3, 1.0f, 0.0f, 1.0f);
}

void UAcidStatusEffect::OnEffectApplied()
{
    AcidStacks = 0;
    if (EnemyRef)
    {
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 0.0f);
        EnemyRef->GetMesh()->SetVectorParameterValueOnMaterials(FName("EffectColor"), StatusEffectColor);
        EnemyRef->GetMesh()->SetVectorParameterValueOnMaterials(FName("EffectColorMax"), AcidMaxColor);
    }
}

void UAcidStatusEffect::UpdateEffect(const FDamageEffects& DamageEffects)
{
    if (DamageEffects.StatusEffectType == EStatusEffectType::Acid)
    {
        AcidStacks = FMath::Clamp(AcidStacks + 1, 0, MaxAcidStacks);

        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), (1.0f * AcidStacks) / MaxAcidStacks);

        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_AcidReduce);
        EnemyRef->GetWorld()->GetTimerManager().SetTimer(TH_AcidReduce, this, &UAcidStatusEffect::UpdateAcidStacks, 1.0f, true);
    }
}

EStatusEffectType UAcidStatusEffect::TrySwitchCurrentState(const FDamageEffects& DamageEffects)
{
    return DamageEffects.StatusEffectType;
}

float UAcidStatusEffect::ProcessDamage(const FDamageEffects& DamageEffects)
{
    return DamageEffects.BaseDamage;
}

void UAcidStatusEffect::BeginDestroy()
{
    ClearAcid();
    EnemyRef = nullptr;
    
    Super::BeginDestroy();
}

void UAcidStatusEffect::ApplyAcidDamage() const
{
    AController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    UGameplayStatics::ApplyDamage(EnemyRef, AcidStacks, PlayerController, PlayerCharacter, UDamageType::StaticClass());   
}

void UAcidStatusEffect::UpdateAcidStacks()
{
    ApplyAcidDamage();
    AcidStacks--;
    
    if (EnemyRef)
    {
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 1.0f * AcidStacks / MaxAcidStacks);
    }

    if (AcidStacks == 0)
    {
        ClearAcid();      
    }
}

void UAcidStatusEffect::ClearAcid()
{
    if (EnemyRef && EnemyRef->GetWorld())
    {
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 0.0f);
        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_AcidReduce);
    }
}
