// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EnemyTargetPoint.generated.h"

class AEnemyParent;

UCLASS()
class BLUNDERSGAME_API AEnemyTargetPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyTargetPoint();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Target Point Weight")
	float GetHiddenFactor() const { return HiddenFactor; }

	UFUNCTION(BlueprintCallable, Category = "Target Point Weight")
    float GetTotalWeight() const;

	UFUNCTION(BlueprintCallable, Category = "Target Point Weight")
    void AddEnemyWeight(AEnemyParent* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Target Point Weight")
	void DecreaseEnemyWeight(AEnemyParent* Enemy);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	class UBillboardComponent *SpriteComponent;
#endif
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Reset() override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent *CollisionSphere;

	UPROPERTY(EditAnywhere, Category = "Point Settings")
	float PointRadius = 100.0;

	UPROPERTY(EditAnywhere, Category = "Point Settings")
	float HiddenFactor = 0.0f;

private:

	UPROPERTY()
	float CurrentEnemyWeight;

	UPROPERTY()
	TArray<TWeakObjectPtr<AEnemyParent>> EnemiesInTargetPoint;
};
