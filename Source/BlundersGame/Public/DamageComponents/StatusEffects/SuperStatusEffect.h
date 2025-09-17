// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.h"
#include "SuperStatusEffect.generated.h"

class ASuperEffectCharge;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API USuperStatusEffect : public UStatusEffectBase
{
	GENERATED_BODY()
public:
    USuperStatusEffect();

    virtual void OnEffectApplied() override;
    
    virtual float ProcessDamage(const FDamageEffects& DamageEffects) override;

    virtual void OwnerDestroyed() override;

    virtual void BeginDestroy() override;
protected:
    UFUNCTION()
    void SuperWearOff();
 
    UPROPERTY()
    FTimerHandle TH_SuperEffectWearOff;

    UPROPERTY()
    TSubclassOf<ASuperEffectCharge> SuperCharge;

    UPROPERTY()
    bool IsSuper = false;

    UPROPERTY()
    float SuperWearOffTime = 4.0f;

    UPROPERTY()
    int SuperDamage = 0;
};
