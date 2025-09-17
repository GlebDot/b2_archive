// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Engine/GameInstance.h"
#include "BlundersGame/Public/PlayerBase/Components/KeyHolderComponent.h"
#include "PlayerWeapons/Base/WeaponBase.h"
#include "PlayerWeapons/Base/WeaponryConfig.h"

#include "BlundersGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerAvailableWeapon
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerWeaponSockets SocketType = EPlayerWeaponSockets::UpSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWeaponSocket> WeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBought = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price = 0;
};

UCLASS()
class BLUNDERSGAME_API UBlundersGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerKeysMap(const TMap<EKeyTypes, uint8> Keys) { KeysMap.Append(Keys); }

	UFUNCTION(BlueprintCallable)
	const TMap<EKeyTypes, uint8>& GetPlayerKeysMap() const { return KeysMap; }

	UFUNCTION(BlueprintCallable)
	int GetCoins() const {return PlayerCoins;}

	UFUNCTION(BlueprintCallable)
	void IncreaseCoins(int IncreaseCoins) {PlayerCoins += IncreaseCoins;}

	UFUNCTION(BlueprintCallable)
	void DecreaseCoins(int DecreaseCoins) {PlayerCoins -= DecreaseCoins;}

	UFUNCTION(BlueprintCallable)
	TArray<FPlayerAvailableWeapon>& GetAvailableWeapons() {return AvailableWeapons;}

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EKeyTypes, uint8> KeysMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EPlayerWeaponSockets, FWeaponSocketData> LightWeaponSockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EPlayerWeaponSockets, FWeaponSocketData> MiddleWeaponSockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EPlayerWeaponSockets, FWeaponSocketData> HeavyWeaponSockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlayerAvailableWeapon> AvailableWeapons;

	UPROPERTY(BlueprintReadWrite);
	int PlayerCoins;
	
};
