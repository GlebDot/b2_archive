// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawner.h"
#include "SpawnParentComponent.h"
#include "Components/ActorComponent.h"
#include "WavesSpawnComponent.generated.h"

class AEnemyParent;

USTRUCT(BlueprintType)
struct FEnemySpawnSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemyParent> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int EnemyCount;
};

USTRUCT(BlueprintType)
struct FEnemyWaveSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WeightThreshold = 0.75;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEnemySpawnSettings> WaveEnemiesSet;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UWavesSpawnComponent : public USpawnParentComponent, public ISpawner
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWavesSpawnComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Spawn")
    void BeginSpawn(AArenaMaster *ArenaMasterRef);
	virtual void BeginSpawn_Implementation(AArenaMaster *ArenaMasterRef) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeightAdded(const float Weight);

	UFUNCTION()
	void OnWeightReduced();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Settings", meta=(DisplayPriority = "0"))
	float SpawnEnemyDelayTime = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Settings", meta=(DisplayPriority = "0"))
	float SpawnBeginDelayTime = 2.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Settings", meta=(DisplayPriority = "0"))
	bool IsDefaultWave = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Settings", meta=(DisplayPriority = "1", EditCondition = "IsDefaultWave"))
	TArray<FEnemySpawnSettings> DefaultWaveSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn|Settings", meta=(DisplayAfter = "DefaultWaveSettings"))
	TArray<FEnemyWaveSettings> WaveSettings;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawn")
	void SpawnEnemyWave();

private:
	UFUNCTION()
	void AllowSpawnNewWave();

	UPROPERTY()
	float CurrentWaveWeight;

	UPROPERTY()
	bool CanSpawnNewWave = false;
	
	UPROPERTY()
	int CurrentWaveSpawnIndex;

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	UPROPERTY()
	FTimerHandle NewWaveTimerHandle;
};
