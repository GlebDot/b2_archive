// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlundersDamageType.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBlundersDamageType : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLUNDERSGAME_API IBlundersDamageType
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Handle")
    float ProcessDamage(float BaseDamage, AActor *DamagedActor) const;
};
