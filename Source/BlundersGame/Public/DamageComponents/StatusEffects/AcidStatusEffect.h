// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffectBase.h"
#include "AcidStatusEffect.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UAcidStatusEffect : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	UAcidStatusEffect();

	virtual void OnEffectApplied() override;

	virtual void UpdateEffect(const FDamageEffects& DamageEffects) override;
    
	virtual EStatusEffectType TrySwitchCurrentState(const FDamageEffects& DamageEffects) override;

	virtual float ProcessDamage(const FDamageEffects& DamageEffects) override;

	virtual void BeginDestroy() override;

protected:
	UPROPERTY()
	int AcidStacks;

	UPROPERTY()
	int MaxAcidStacks = 13;
	
	UPROPERTY()
	FTimerHandle TH_AcidReduce;

	UPROPERTY()
	FVector4 AcidMaxColor;

	UFUNCTION()
	void ApplyAcidDamage() const;
	
	UFUNCTION()
	void UpdateAcidStacks();

	UFUNCTION()
	void ClearAcid();
};
