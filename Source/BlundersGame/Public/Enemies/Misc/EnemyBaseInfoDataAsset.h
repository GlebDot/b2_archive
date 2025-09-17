// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enemies/EnemyBaseCharacter.h"
#include "Enemies/EnemyArmour/ArmourContainerComponent.h"
#include "Engine/DataAsset.h"
#include "PlayerBase/ResourcePickUps/PickUpSpawner.h"

#include "EnemyBaseInfoDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BLUNDERSGAME_API UEnemyBaseInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base")
    float EnemyWeight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base")
    EEnemyJumpResponse JumpResponse;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base")
    EPickUpType PickUpType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base")
    EPickUpAmount PickUpAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DamageHandler")
    float MaxHealth = 13.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DamageHandler")
    float StunTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DamageHandler")
    float StunCap = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Armour")
    float ArmourMaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Armour")
    TArray<FArmourPlateSettings> ArmourSettings;
};
