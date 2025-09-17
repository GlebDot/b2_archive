// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffectBase.generated.h"

struct FDamageEffects;
class AEnemyBaseCharacter;

UENUM()
enum class EStatusEffectType : uint8
{
    Default UMETA(DisplayName = "Default"),
    Heat UMETA(DisplayName = "Heat"),
    Cold UMETA(DisplayName = "Cold"),
    Acid UMETA(DisplayName = "Acid"),
    Super UMETA(DisplayName = "Super")
};

/**
 * 
 */
UCLASS(Abstract)
class BLUNDERSGAME_API UStatusEffectBase : public UObject
{
	GENERATED_BODY()
public:
    UStatusEffectBase(){}

    virtual void OnEffectApplied() {};

    virtual EStatusEffectType TrySwitchCurrentState(const FDamageEffects& DamageEffects) { return StatusEffectType; };

    virtual float ProcessDamage(const FDamageEffects& DamageEffects) { return 0.0; };

    virtual void UpdateEffect(const FDamageEffects& DamageEffects) {};

    virtual void OwnerDestroyed() {};

    EStatusEffectType GetStatusEffectType() const { return StatusEffectType; }

    void SetEnemyRef(AEnemyBaseCharacter* Enemy) {EnemyRef = Enemy;}

protected:
    UPROPERTY()
    AEnemyBaseCharacter *EnemyRef;
    
    UPROPERTY()
    EStatusEffectType StatusEffectType = EStatusEffectType::Default;

    UPROPERTY()
    FVector4 StatusEffectColor = FVector4(0.0f);
};


