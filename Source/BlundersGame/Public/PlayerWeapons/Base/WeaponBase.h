// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DamageComponents/Damagable.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UWeaponSocket;
class APlayerCharacter;
struct FWeaponSettings;
struct FWeaponSocketData;

UENUM(BlueprintType)
enum class EPlayerWeaponSockets : uint8
{
	UpSocket UMETA(DisplayName = "UpSocket"),
    DawnSocket UMETA(DisplayName = "DawnSocket"),
};

UCLASS()
class BLUNDERSGAME_API UWeaponBase : public UActorComponent
{
	GENERATED_BODY()
	
public:
	struct FWeaponEquipInfo
	{
		bool bInfoVisible = false;
		uint8 WeaponByteSocket : 1;
	};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Sockets")
	USkeletalMeshComponent *WeaponBaseMesh;
	
	UWeaponBase();

	void InitConstructor(const FWeaponSettings& InWeaponSettings, USceneComponent* InAttachToComponent);
	void Fire();
	void StopFire();
	void SwitchSocket();
	void SetWeaponInfoVisibility(bool bVisible);

	UFUNCTION()
	void SocketMontagePlayed();

	APlayerCharacter* GetPlayer() const;
	void SetPlayer(APlayerCharacter* InPlayer);
	EPlayerWeaponSockets GetActiveWeaponSocket() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponInfoVisible();

	void GetSocketsInfo(TArray<FDamageEffects> &Info);

	void RemoveWeapon();
	void EquipWeapon(const FWeaponEquipInfo& InWeaponInfo);
	FWeaponEquipInfo GetWeaponEquipInfo() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sockets")
	EAttachmentRule SocketSnapRuleRotation = EAttachmentRule::SnapToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sockets")
	EAttachmentRule SocketSnapRuleLocation = EAttachmentRule::SnapToTarget;

	//TODO: remove under [B1-21]
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Animation")
	UAnimMontage* PlayerFireMontage;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	UWeaponSocket* UpSocket;

	UPROPERTY()
	UWeaponSocket* DawnSocket;

	UPROPERTY()
	UWeaponSocket* CurrentSocket;

	UPROPERTY()
	APlayerCharacter* Player;

	uint8 bWeaponInfoVisible : 1;
	uint8 bWeaponEquipped : 1;
	uint8 SocketByte : 1; // 0 - means first socket, 1 - means second socket
	
	void SwitchSocket(bool InSocketByte);
	void CreateVisualComponents(const FWeaponSettings& InWeaponSettings, USceneComponent* InAttachToComponent);

	void SpawnSockets(
		const FWeaponSocketData& InUpSocketSettings,
		const FWeaponSocketData& InDawnSocketSettings);
	void ClearSockets();
};
