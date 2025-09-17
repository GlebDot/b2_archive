// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/StatusEffects/ColdStatusEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BlundersGame/Public/Enemies/EnemyBaseCharacter.h"
#include "DamageComponents/DamageTypes.h"
#include "Enemies/EnemyParent.h"
#include "Kismet/GameplayStatics.h"

UColdStatusEffect::UColdStatusEffect()
{
    StatusEffectType = EStatusEffectType::Cold;
    StatusEffectColor = FVector4(0.0f, 0.3f, 1.0f, 1.0f);

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ShotEffect(TEXT("NiagaraSystem'/Game/Enemies/_Base/FX/EnemyCloseRangeAttack_FX.EnemyCloseRangeAttack_FX'"));
    if (ShotEffect.Succeeded())
    {
        AdditionalShotEffect = ShotEffect.Object;
    }
}

void UColdStatusEffect::OnEffectApplied()
{
    IsEnemyCold = true;
    
    if (EnemyRef)
    {
        EnemyRef->CustomTimeDilation = 0.65f;
        
        EnemyRef->GetMesh()->SetVectorParameterValueOnMaterials(FName("EffectColorMax"), StatusEffectColor);
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 1.0f);

        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_ColdUpdate);
        EnemyRef->GetWorld()->GetTimerManager().SetTimer(TH_ColdUpdate, this, &UColdStatusEffect::ClearCoolEffect, CoolTime, false);
    }
}

void UColdStatusEffect::UpdateEffect(const FDamageEffects& DamageEffects)
{
    if (DamageEffects.StatusEffectType == EStatusEffectType::Heat && DamageEffects.StatusEffectStrength > 0.0f && EnemyRef)
    {
        float CoolTimeRemaining = EnemyRef->GetWorld()->GetTimerManager().GetTimerRemaining(TH_ColdUpdate);
        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_ColdUpdate);

        CoolTimeRemaining = CoolTimeRemaining - CoolTime * DamageEffects.StatusEffectStrength;

        if (CoolTimeRemaining < 0.75f)
        {
            ClearCoolEffect();
        } else
        {
            EnemyRef->GetWorld()->GetTimerManager().SetTimer(TH_ColdUpdate, this, &UColdStatusEffect::ClearCoolEffect, CoolTimeRemaining, false);
        }
    }

    if (DamageEffects.StatusEffectType == EStatusEffectType::Cold)
    {
        OnEffectApplied();
    }
}

EStatusEffectType UColdStatusEffect::TrySwitchCurrentState(const FDamageEffects& DamageEffects)
{  
    if (!IsEnemyCold)
    {
        return DamageEffects.StatusEffectType;
    }

    return StatusEffectType;
}

float UColdStatusEffect::ProcessDamage(const FDamageEffects& DamageEffects)
{
    float FinalDamage = DamageEffects.BaseDamage;
    if ((DamageEffects.DamageType == EDamageType::Energy || DamageEffects.DamageType == EDamageType::DoubleMatter)
        && IsEnemyCold && DamageEffects.DamageTypeStrength > 0.0f)
    {
        FinalDamage = DamageEffects.BaseDamage * DamageEffects.DamageTypeStrength;
        ApplySplitDamage(FinalDamage);
    }
    
    return FinalDamage;
}

void UColdStatusEffect::BeginDestroy()
{
    ClearCoolEffect();
    EnemyRef = nullptr;
    
    Super::BeginDestroy();
}

void UColdStatusEffect::ClearCoolEffect()
{
    IsEnemyCold = false;
    if (EnemyRef && EnemyRef->GetWorld())
    {
        EnemyRef->CustomTimeDilation = 1.0f;
        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_ColdUpdate);
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 0.0f);
    }
}

void UColdStatusEffect::ApplySplitDamage(const float SplitDamage) const
{
    TArray<AActor*> Enemies;
    UGameplayStatics::GetAllActorsOfClass(EnemyRef->GetWorld(), AEnemyParent::StaticClass(), Enemies);
    Enemies.Remove(EnemyRef);
    if (Enemies.Num() < 1)
    {
        return;
    }

    AEnemyParent* EnemyToDamage = nullptr;
    int MinEnemyWeight = 1000000.0f;
    for (auto& Enemy : Enemies)
    {
        AEnemyParent *Tmp = Cast<AEnemyParent>(Enemy);
        if (MinEnemyWeight > Tmp->GetEnemyWeight())
        {
            MinEnemyWeight = Tmp->GetEnemyWeight();
            EnemyToDamage = Tmp;
        }
    }

    if (EnemyToDamage)
    {
        UGameplayStatics::ApplyDamage(EnemyToDamage, SplitDamage, EnemyRef->GetController(), EnemyRef, UDamageType::StaticClass());
	
        const FVector SpawnLocation = EnemyRef->GetActorLocation();
        UNiagaraComponent *NiagaraSystemRef = UNiagaraFunctionLibrary::SpawnSystemAtLocation(EnemyRef->GetWorld(), AdditionalShotEffect, SpawnLocation);
        NiagaraSystemRef->SetNiagaraVariableVec3(FString("Beam End"), EnemyToDamage->GetActorLocation());
    }
}
