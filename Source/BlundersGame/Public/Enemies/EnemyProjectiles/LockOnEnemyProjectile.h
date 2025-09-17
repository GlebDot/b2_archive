// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyProjectiles/EnemyProjectile.h"
#include "LockOnEnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API ALockOnEnemyProjectile final : public AEnemyProjectile
{
	GENERATED_BODY()

public:
    ALockOnEnemyProjectile();
    
    virtual void BeginPlay() override;

    virtual void BeginDestroy() override;

    virtual void Tick(float DeltaSeconds) override;

protected:
    UFUNCTION()
    void ActivateProjectile();

    UPROPERTY()
    FTimerHandle TH_Activation;
    
    UPROPERTY(EditDefaultsOnly, Category="Projectile Settings|Lock On")
    float SpeedAfterActivation = 3000.0f;

    UPROPERTY(EditDefaultsOnly, Category="Projectile Settings|Lock On")
    float ActivationTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category="Projectile Settings|Lock On")
    float ProjectileSpeedDecreasingRate = 1.0f;
};
