// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnParentComponent.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "BurstSpawnComponent.generated.h"

class AArenaMaster;
class AEnemySpawnPoint;

USTRUCT(BlueprintType)
struct FBurstSpawnEnemySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemyParent> EnemyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int EnemyCount = 0;
};

UENUM()
enum class EBurstSpawnType : uint8
{
	SingleBurst UMETA(DisplayName = "Single Burst"),
	DoubleBurst UMETA(DisplayName = "Double Burst"),
	TripleBurst UMETA(DisplayName = "Triple Burst")
};


UCLASS( ClassGroup=(EnemySpawn), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UBurstSpawnComponent : public USpawnParentComponent, public ISpawner
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBurstSpawnComponent();

	//Spawning Component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Spawn")
    void BeginSpawn(AArenaMaster *ArenaMasterRef);
	virtual void BeginSpawn_Implementation(AArenaMaster *ArenaMasterRef) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Settings|Enemy Array")
	TArray<FBurstSpawnEnemySettings> EnemyArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Settings|Spawn Flow Settings")
	float SpawnBurstsDelayTime = 2.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Settings|Spawn Flow Settings")
	float SpawnEnemiesDelayTime = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena Settings|Spawn Flow Settings")
	EBurstSpawnType SpawnType = EBurstSpawnType::SingleBurst;

private:
	
	UFUNCTION()
	void SpawnBurst();

	UPROPERTY()
	FTimerHandle SpawnDelayTimerHandle;

	UPROPERTY()
	int SpawnBurstCounter = 0;

	UPROPERTY()
	int SpawnEnemyTypesAmount = 0;
};
