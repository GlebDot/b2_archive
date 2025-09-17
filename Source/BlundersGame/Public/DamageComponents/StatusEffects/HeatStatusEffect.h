// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffectBase.h"
#include "HeatStatusEffect.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UHeatStatusEffect : public UStatusEffectBase
{
	GENERATED_BODY()
public:
    UHeatStatusEffect();

    virtual void OnEffectApplied() override;

    virtual void UpdateEffect(const FDamageEffects& DamageEffects) override;
    
    virtual EStatusEffectType TrySwitchCurrentState(const FDamageEffects& DamageEffects) override;

    virtual float ProcessDamage(const FDamageEffects& DamageEffects) override;

    virtual void BeginDestroy() override;

protected:
    UPROPERTY()
    float CurrentHeatPercent = 0.0;

    UPROPERTY()
    float HeatUpdateDeltaTime = 0.1f;

    UPROPERTY()
    FTimerHandle TH_HeatPercent;

    UPROPERTY()
    FVector4 HeatMaxColor;
    
    UFUNCTION()
    void UpdateHeatPercent();
};
