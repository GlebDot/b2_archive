// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapons/Base/WeaponSocketTypes_Base.h"
#include "WeaponSocketTypes_Projectile.generated.h"

class AWeaponProjectile;

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeaponProjectile> WeaponProjectile {};
};

UCLASS()
class UWeaponSocketSettings_Projectile : public UWeaponSocketSettings_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsProjectile ProjectileSettings {};
};
