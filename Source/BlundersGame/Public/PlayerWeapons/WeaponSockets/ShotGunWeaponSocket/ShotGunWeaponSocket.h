// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapons/WeaponSockets/HitScanWeaponSocket/HitScanWeaponSocket.h"
#include "ShotGunWeaponSocket.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UShotGunWeaponSocket : public UHitScanWeaponSocket
{
	GENERATED_BODY()

public:
	virtual void FireProjectile() override;

protected:
	UFUNCTION()
	void HitScanPellets(const FVector &StartLocation, const FVector &Direction);
};
