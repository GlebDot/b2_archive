// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponents/DamageTypes.h"
#include "Engine/DataAsset.h"
#include "PlayerBase/Components/PlayerAmmoComponent.h"
#include "WeaponSocketTypes_Base.generated.h"

class UWeaponSocketUserWidget;
enum class EAmmoRequestType : uint8;
class USoundCue;
class UCameraShakeBase;
class UCurveFloat;
class UDamageType;
class USkeletalMesh;
class UWeaponSocket;

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpreadAngle = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1))
	int32 BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BulletsPerMinute = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWeaponSocketUserWidget> SocketWidgetClass {};

	UPROPERTY(EditDefaultsOnly)
	FName ShotSocketName = FName(TEXT("ShotSocket"));
};

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsAimHelper
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TraceDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InnerCircleAngle = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MiddleCircleAngle = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OuterCircleAngle = 35.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 TracesCount = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsDebug = false;
};

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsFeeling
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float PitchRecoilAmount = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* PitchCurve;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> WeaponCameraShake;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* WeaponSound;
};

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsAmmo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 AmmoPerShot = 3;

	UPROPERTY(EditDefaultsOnly)
	EAmmoRequestType AmmoType = EAmmoRequestType::Light;
};

USTRUCT(BlueprintType)
struct FWeaponSocketSettingsDamage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Settings")
	FDamageEffects ShotDamageSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Settings")
	TSubclassOf<UDamageType> DamageType;
	
};

UCLASS()
class UWeaponSocketSettings_Base : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsBase BaseSettings {};

	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsDamage DamageSettings {};
	
	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsAimHelper AimHelperSettings {};

	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsAmmo AmmoSettings {};

	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketSettingsFeeling FeelingSettings {};
};
