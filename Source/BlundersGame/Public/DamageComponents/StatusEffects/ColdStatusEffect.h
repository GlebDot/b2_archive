// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.h"
#include "ColdStatusEffect.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UColdStatusEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
    UColdStatusEffect();

    virtual void OnEffectApplied() override;

    virtual void UpdateEffect(const FDamageEffects& DamageEffects) override;
    
    virtual EStatusEffectType TrySwitchCurrentState(const FDamageEffects& DamageEffects) override;

    virtual float ProcessDamage(const FDamageEffects& DamageEffects) override;

    virtual void BeginDestroy() override;
protected:
    UFUNCTION()
    void ClearCoolEffect();

    UFUNCTION()
    void ApplySplitDamage(const float SplitDamage) const;
   
    UPROPERTY()
    FTimerHandle TH_ColdUpdate;

    UPROPERTY()
    bool IsEnemyCold = false;

    UPROPERTY()
    float CoolTime = 10.0f;

    UPROPERTY()
    UNiagaraSystem *AdditionalShotEffect;
};
