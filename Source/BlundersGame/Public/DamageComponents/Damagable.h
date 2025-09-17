// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"


struct FDamageEffects;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

class IDamagable
{
	GENERATED_BODY()

public:
	virtual void ReceiveDamage(const FDamageEffects& DamageEffects) = 0;
};
