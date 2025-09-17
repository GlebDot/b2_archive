// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

class UEnemyProjectileMovementComponent;

UCLASS()
class BLUNDERSGAME_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

	virtual void Reset() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnemyProjectileMovementComponent *ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent *SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Settings|General")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Settings|General")
	float BaseDamage = 10.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Settings|Homing")
	bool IsHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Settings|Homing", meta=(EditCondition="IsHoming"))
	float HomingForce = 1000.0;

	UFUNCTION()
	void OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit );

	UFUNCTION()
	void SetTarget(AActor *Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor *ProjectileTarget;

};
