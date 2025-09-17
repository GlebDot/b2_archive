// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponents/Explosions/ExplosionBasic.h"
#include "Enemies/EnemyParent.h"
#include "Kam.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AKam : public AEnemyParent
{
	GENERATED_BODY()


public:
	AKam();
	virtual void OnActorDestroyed(AActor* DestroyedActor) override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	UStaticMeshComponent *HintMesh;
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void CalculateDistanceToPlayer();

	UPROPERTY()
	bool IsHintOnPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kam Settings", meta=(DisplayPriority = "0"))
	float ExplosionDistance = 450.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kam Settings", meta=(DisplayPriority = "1"))
	TSubclassOf<AExplosionBasic> ExplosionType;
};
