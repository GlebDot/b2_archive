// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyProjectileMovementComponent.generated.h"

DECLARE_DELEGATE(FOnProjectileFlyOverTargetDelegate)

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	virtual FVector ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const override;

	virtual void OnComponentCreated() override;

protected:
	UFUNCTION()
	void OnProjectileFlyOver();
	
	FOnProjectileFlyOverTargetDelegate OnProjectileFlyOverTargetDelegate;
};
