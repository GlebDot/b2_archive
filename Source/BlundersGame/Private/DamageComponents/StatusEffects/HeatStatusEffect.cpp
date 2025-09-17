// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/StatusEffects/HeatStatusEffect.h"
#include "BlundersGame/Public/Enemies/EnemyBaseCharacter.h"
#include "DamageComponents/DamageTypes.h"

UHeatStatusEffect::UHeatStatusEffect()
{
    StatusEffectColor = FVector4(1.0f, 0.075f, 0.0f, 1.0f);
    HeatMaxColor = FVector4(1.0f, 0.0f, 0.3f, 1.0f);
    StatusEffectType = EStatusEffectType::Heat;
}

void UHeatStatusEffect::OnEffectApplied()
{
    CurrentHeatPercent = 0.0f;
    if (EnemyRef)
    {
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), CurrentHeatPercent);
        EnemyRef->GetMesh()->SetVectorParameterValueOnMaterials(FName("EffectColor"), StatusEffectColor);
        EnemyRef->GetMesh()->SetVectorParameterValueOnMaterials(FName("EffectColorMax"), HeatMaxColor);
    }
}

void UHeatStatusEffect::UpdateEffect(const FDamageEffects& DamageEffects)
{
    if (DamageEffects.StatusEffectType == EStatusEffectType::Heat && DamageEffects.StatusEffectStrength > 0.0f)
    {
        CurrentHeatPercent += DamageEffects.StatusEffectStrength;
        CurrentHeatPercent = FMath::Clamp(CurrentHeatPercent, 0.0f, 1.0f);

        float HeatPercentUpdateDelay = 1.0f;
        if (CurrentHeatPercent == 1.0f)
        {
            HeatPercentUpdateDelay = 2.0f;
        }

        if (EnemyRef)
        {
            EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), CurrentHeatPercent);
        
            EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_HeatPercent);
            EnemyRef->GetWorld()->GetTimerManager().SetTimer(TH_HeatPercent, this, &UHeatStatusEffect::UpdateHeatPercent, HeatUpdateDeltaTime, true, HeatPercentUpdateDelay);
        }
    }
}

EStatusEffectType UHeatStatusEffect::TrySwitchCurrentState(const FDamageEffects& DamageEffects)
{
    if (DamageEffects.StatusEffectType == EStatusEffectType::Cold && CurrentHeatPercent == 1.0f)
    {
        return EStatusEffectType::Super;
    }

    return DamageEffects.StatusEffectType;
}

float UHeatStatusEffect::ProcessDamage(const FDamageEffects& DamageEffects)
{
    float FinalDamage = DamageEffects.BaseDamage;

    if ((DamageEffects.DamageType == EDamageType::Plasma || DamageEffects.DamageType == EDamageType::DoubleMatter) && DamageEffects.DamageTypeStrength > 0.0f)
    {
        FinalDamage = FinalDamage + FinalDamage * 4.0f * DamageEffects.DamageTypeStrength * CurrentHeatPercent;
    }
    
    return  FinalDamage;
}

void UHeatStatusEffect::BeginDestroy()
{
    if (EnemyRef && EnemyRef->GetWorld())
    {
        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_HeatPercent);
    }

    EnemyRef = nullptr;

    Super::BeginDestroy();
}

void UHeatStatusEffect::UpdateHeatPercent()
{
    CurrentHeatPercent = FMath::FInterpConstantTo(CurrentHeatPercent, 0.0f, HeatUpdateDeltaTime, 0.1);
    if (EnemyRef)
    {
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), CurrentHeatPercent);
        if (CurrentHeatPercent == 0.0f)
        {
            EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_HeatPercent);
        }
    }
}


