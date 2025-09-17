// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerWeapons/Base/WeaponSocketTypes_Base.h"
#include "CoreMinimal.h"
#include "WeaponSocketTypes_HitScan.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsVisual_HitScan
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* TrailFX = nullptr;
};

UCLASS()
class UWeaponSocketSettings_HitScan : public UWeaponSocketSettings_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsVisual_HitScan VisualSettings {};
};
