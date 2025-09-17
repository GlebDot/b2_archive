// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffectBase.h"
#include "UObject/Object.h"
#include "DamageTypes.generated.h"

enum class EStatusEffectType : uint8;

UENUM()
enum class EDamageType : uint8
{
	ZeroMatter UMETA(DisplayName = "ZeroMatter"),
	Plasma UMETA(DisplayName = "Plasma"),
	Energy UMETA(DisplayName = "Energy"),
	DoubleMatter UMETA(DisplayName = "DoubleMatter"),
	Cigarette UMETA(DisplayName = "Cigarette")
};

USTRUCT(BlueprintType)
struct FDamageEffects
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Stop = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDamageType DamageType = EDamageType::ZeroMatter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin="0.0", ClampMax="1.0"))
	float DamageTypeStrength = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EStatusEffectType StatusEffectType = EStatusEffectType::Default;

	/*Optional: Allows to modify the strength of some effects such as heat*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin="0.0", ClampMax="1.0"))
	float StatusEffectStrength = 0.0f;

	void operator+=(const FDamageEffects& Other);

	void operator=(const FDamageEffects& Other);

	FDamageEffects() = default;

	FDamageEffects(
		float Damage,
		float Stop,
		EDamageType DmgType,
		float DmgTypeStrength,
		EStatusEffectType StatusEffect,
		float StatusStrength);
};
