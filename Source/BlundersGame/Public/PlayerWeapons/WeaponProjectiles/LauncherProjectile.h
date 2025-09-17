// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapons/Base/WeaponProjectile.h"
#include "LauncherProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API ALauncherProjectile : public AWeaponProjectile
{
	GENERATED_BODY()
public:
	ALauncherProjectile();
protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
