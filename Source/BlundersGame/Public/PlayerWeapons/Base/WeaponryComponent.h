// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "CoreMinimal.h"
#include "WeaponryComponent.generated.h"


enum class EWeaponIndex : uint8
{
	Idx1 = 0,
	Idx2 = 1,
	Idx3 = 2,
	Idx4 = 3,
	Idx5 = 4,
	Idx6 = 5,
	Idx7 = 6,
	Idx8 = 7,
};

class UWeaponryConfig;
class APlayerCharacter;

UCLASS()
class BLUNDERSGAME_API UWeaponryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	void Init(APlayerCharacter* InPlayer);
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	void SwitchWeapon(EWeaponIndex WeaponIndex);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	UWeaponryConfig* WeaponryConfig;

private:
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	
	UPROPERTY()
	UWeaponBase* CurrentWeapon;

	UPROPERTY()
	TArray< UWeaponBase* > Weapons {};

	bool bInCombatState = false;

	void ClearWeapons();
	void SpawnWeapons();
	void SetupWeapons();

	void FirePressed();
	void FireReleased();
	void WeaponSwitch1Pressed();
	void WeaponSwitch2Pressed();
	void WeaponSwitch3Pressed();
	void SwitchSocketPressed();
	void PlayerSwitchedCombatState(bool bInState);
};
