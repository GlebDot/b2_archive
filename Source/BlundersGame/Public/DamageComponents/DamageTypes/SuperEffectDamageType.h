// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BlundersDamageType.h"
#include "GameFramework/DamageType.h"
#include "SuperEffectDamageType.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API USuperEffectDamageType : public UDamageType, public IBlundersDamageType
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage processing")
    float ProcessDamage(float BaseDamage, AActor *DamagedActor) const;
	virtual float ProcessDamage_Implementation(float BaseDamage, AActor *DamagedActor) const override;
};
