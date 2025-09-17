// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawner.generated.h"

class AArenaMaster;

UENUM()
enum class EEnemyTypes : uint8
{
	Kamikadze UMETA(DisplayName = "Kamikadze"),
	HeapWalking UMETA(DisplayName = "HeapWalking"),
	Meat UMETA(DisplayName = "Meat"),
	Shooter UMETA(DisplayName = "Shooter"),
	Runner UMETA(DisplayName = "Runner"),
	Rocket UMETA(DisplayName = "Rocket")
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLUNDERSGAME_API ISpawner
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Spawn")
	void BeginSpawn(AArenaMaster *ArenaMasterRef);
};
