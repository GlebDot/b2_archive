// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSocketTypes_HitScan.h"
#include "PlayerWeapons/Base/WeaponSocket.h"
#include "BlundersGame/Public/DamageComponents/Damagable.h"
#include "DamageComponents/DamageTypes.h"
#include "HitScanWeaponSocket.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UHitScanWeaponSocket : public UWeaponSocket
{
	GENERATED_BODY()

public:

    virtual void FireProjectile() override;
protected:

    virtual void SetupWeaponSettings(UWeaponSocketSettings_Base* InWeaponSettings) override;
    void HitScan(const FVector &StartLocation, const FVector &EndLocation);

    UPROPERTY(BlueprintReadOnly)
    FWeaponSocketSettingsVisual_HitScan SocketSettingsVisual {};

private:
    UPROPERTY()
    AActor *DamageCauser;

    UPROPERTY()
    AController *DamageInstigator;

    void SimpleDamage(AActor *ActorToDamage);

    void SpawnNiagaraSystem(const FVector &BeamEnd) const;
};
