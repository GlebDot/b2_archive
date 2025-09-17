// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponryConfig.generated.h"

class UWeaponSocketSettings_Base;
class USkeletalMesh;
class UWeaponBase;
class UWeaponSocket;

USTRUCT(BlueprintType)
struct FWeaponSocketData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponSocket> WeaponSocket;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* SocketMesh;

	UPROPERTY(EditDefaultsOnly)
	UWeaponSocketSettings_Base* WeaponSocketSettings;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FWeaponSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UWeaponBase> Weapon;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketData UpSocketSettings {};

	UPROPERTY(EditDefaultsOnly)
	FWeaponSocketData DawnSocketSettings {};

	bool IsValid() const;
};

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UWeaponryConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FWeaponSettings> WeaponSettings {};
};
