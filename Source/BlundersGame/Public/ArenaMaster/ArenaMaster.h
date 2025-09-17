// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaMaster.generated.h"


class AEnemyTargetPoint;
class AEnemySpawnPoint;
class AEnemyParent;

DECLARE_DELEGATE_OneParam(FWeightReceivedDelegate, float)
DECLARE_DELEGATE(FWeightReducedDelegate)

DECLARE_MULTICAST_DELEGATE(FArenaStartDelegate)
DECLARE_MULTICAST_DELEGATE(FArenaEndDelegate)

UCLASS()
class BLUNDERSGAME_API AArenaMaster : public AActor
{
	GENERATED_BODY()
	
public:
	
	FORCEINLINE TArray<AEnemySpawnPoint*>& GetSpawnPoints() {return SpawnPoints;}

	FORCEINLINE TArray<AEnemyTargetPoint*>& GetTargetPoints() {return TargetPoints;}

	UFUNCTION(BlueprintCallable, Category = "Enemy Control")
	void AddTickets(int Tickets);

	UFUNCTION(BlueprintCallable, Category = "Enemy Control")
	void DecreaseTickets(int Tickets);

	UFUNCTION(BlueprintCallable, Category = "Enemy Control")
	int GetTickets() const;
	
	// Sets default values for this actor's properties
	AArenaMaster();

	UFUNCTION()
	void UnRegisterEnemy(AEnemyParent *Enemy);

	UFUNCTION()
	void RegisterEnemy(AEnemyParent *Enemy);

	UFUNCTION(BlueprintCallable, Category = "ArenaMaster|Weight")
	void AddWeight(float Weight);

	UFUNCTION(BlueprintCallable, Category = "ArenaMaster|Weight")
	void DecreaseWeight(float Weight);

	UFUNCTION(BlueprintCallable, Category = "ArenaMaster|Weight")
	float GetArenaWeight() const;

	UFUNCTION(BlueprintCallable, Category = "ArenaMaster|Weight")
	bool ArenaTicketsCheck(int TicketsMin) const;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawn")
	void SpawnStart();

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawn")
	void SpawnEnd();

	FWeightReceivedDelegate WeightReceivedDelegate;
	FWeightReducedDelegate WeightReducedDelegate;

	FArenaEndDelegate ArenaEndDelegate;
	FArenaStartDelegate ArenaStartDelegate;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Reset() override;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display)
	class UBillboardComponent* SpriteComponent;
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Control|Navigation")
	int TargetPointsCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Control|Navigation")
	TArray<AEnemyTargetPoint*> TargetPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Control|Spawn")
	int SpawnPointsCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Control|Spawn")
	TArray<AEnemySpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Control|Game Flow")
	int ArenaTickets = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Control")
	TArray<AEnemyParent*> SpawnedEnemies;

private:
	UPROPERTY()
	UActorComponent *SpawnComponentRef;

	UPROPERTY()
	float ArenaWeightSum = 0;

	UPROPERTY()
	bool IsSpawnEnded = false;

	UPROPERTY()
	bool IsArenaEnded = false;
};
