// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSocketTypes_Projectile.h"
#include "PlayerWeapons/Base/WeaponSocket.h"
#include "BlundersGame/Public/PlayerWeapons/Base/WeaponProjectile.h"
#include "ProjectileBasedWeaponSocket.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UProjectileBasedWeaponSocket : public UWeaponSocket
{
	GENERATED_BODY()
public:

	virtual void FireProjectile() override;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon Projectile")
	AWeaponProjectile *LastProjectileRef;

protected:
	virtual void SetupWeaponSettings(UWeaponSocketSettings_Base* InWeaponSettings) override;
	AWeaponProjectile *SpawnProjectile(const FVector &EndLocation, const float Spread = 0.0f);

	UPROPERTY(BlueprintReadOnly)
	FWeaponSocketSettingsProjectile SocketSettingsProjectile {};
};
