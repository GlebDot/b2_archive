// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapons/Base/WeaponProjectile.h"
#include "Components/CapsuleComponent.h"
#include "OverlappingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AOverlappingProjectile : public AWeaponProjectile
{
	GENERATED_BODY()
public:
    AOverlappingProjectile();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCapsuleComponent *CapsuleCollision;
    
protected:
    UFUNCTION()
    void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    virtual void BeginPlay() override;
};
