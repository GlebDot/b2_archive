// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlundersDamageType.h"
#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "PlayerDamageType.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerDamageType : public UDamageType, public IBlundersDamageType
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage processing")
    float ProcessDamage(float BaseDamage, AActor *DamagedActor) const;
	virtual float ProcessDamage_Implementation(float BaseDamage, AActor *DamagedActor) const override;
};
