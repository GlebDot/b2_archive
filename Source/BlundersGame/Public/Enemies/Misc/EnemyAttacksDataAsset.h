// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "EnemyAttacksDataAsset.generated.h"


UENUM()
enum class EEnemyAbilityType : uint8
{
	Primary UMETA(DisplayName="Primary"),
	Secondary UMETA(DisplayName="Secondary"),
	Special UMETA(DisplayName="Special"),
	Stun UMETA(DisplayName="Stun"),
};

USTRUCT(BlueprintType)
struct FEnemyAbilitySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyAbilitySettings")
	UAnimMontage *Montage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyAbilitySettings")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyAbilitySettings")
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyAbilitySettings")
	TSubclassOf<UDamageType> DamageType;
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyAttacksDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="EnemyAbilitiesSettings")
	FEnemyAbilitySettings &GetCurrentAbilitySettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyAbilitiesSettings")
	EEnemyAbilityType CurrentActiveAbilityType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EnemyAbilitiesSettings")
	TMap<EEnemyAbilityType, FEnemyAbilitySettings> EnemyAbilities;

	
};
