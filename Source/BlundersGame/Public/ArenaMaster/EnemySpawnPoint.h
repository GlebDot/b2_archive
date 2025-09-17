// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.generated.h"

class AArenaMaster;
class AEnemyParent;

USTRUCT(BlueprintType)
struct FEnemySpawnEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AEnemyParent> EnemyType;

	UPROPERTY()
	float DelayTime;

	FEnemySpawnEntry() = default;

	FEnemySpawnEntry(const TSubclassOf<AEnemyParent> Enemy, float Delay)
	{
		EnemyType = Enemy;
		DelayTime = Delay;
	}
};

UCLASS()
class BLUNDERSGAME_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnPoint();

	virtual void Reset() override;

	UFUNCTION()
	void SetArenaMaster(AArenaMaster *Arena);

	UFUNCTION()
	AArenaMaster *GetArenaMaster() const;

	UFUNCTION()
	void AddEnemyEntryToSpawn(const TSubclassOf<AEnemyParent> &EnemyType, float GlobalDelayTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)\
	USceneComponent *SceneComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display)
	class UBillboardComponent* SpriteComponent;
#endif

private:

	UFUNCTION()
	void SpawnEnemy();
	
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
	
	UPROPERTY()
	TArray<FEnemySpawnEntry> EnemySpawnQueue;

	UPROPERTY()
	AArenaMaster *ArenaMasterRef;
};
