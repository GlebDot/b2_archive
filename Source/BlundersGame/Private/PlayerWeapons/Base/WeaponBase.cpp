// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/Base/WeaponBase.h"

#include "Blueprint/UserWidget.h"
#include "BlundersGame/Public/PlayerWeapons/Base/WeaponSocket.h"
#include "Components/SkeletalMeshComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "PlayerWeapons/Base/WeaponryConfig.h"

namespace FWeaponBaseLocal
{
	struct FWeaponSocketSpawnContext
	{
		TWeakObjectPtr<APlayerCharacter> Player {};
		TWeakObjectPtr<UPrimitiveComponent> AttachToComponent {};
		TWeakObjectPtr<UWeaponBase> WeaponBase {};
		bool IsValid() const
		{
			return AttachToComponent.IsValid() && WeaponBase.IsValid() && Player.IsValid();
		}
	};
	
	UWeaponSocket* CreateWeaponSocket(const FWeaponSocketSpawnContext& Context, const FWeaponSocketData& InSettings)
	{
		check(Context.IsValid());
		check(InSettings.IsValid());

		APlayerCharacter* Player = Context.Player.Get();
		UWeaponSocket* WeaponSocket = NewObject<UWeaponSocket>(Player, InSettings.WeaponSocket);
		WeaponSocket->RegisterComponent();
		WeaponSocket->Init(
			InSettings,
			Context.AttachToComponent.Get(),
			Context.WeaponBase.Get(),
			Player);

		return WeaponSocket;
	};
}

UWeaponBase::UWeaponBase()
{
	SocketByte = 0;
	bWeaponEquipped = 0;
	bWeaponInfoVisible = 0;
}

void UWeaponBase::InitConstructor(const FWeaponSettings& InWeaponSettings, USceneComponent* InAttachToComponent)
{
	CreateVisualComponents(InWeaponSettings, InAttachToComponent);
	ClearSockets();
	SpawnSockets(InWeaponSettings.UpSocketSettings, InWeaponSettings.DawnSocketSettings);
}

EPlayerWeaponSockets UWeaponBase::GetActiveWeaponSocket() const
{
	return SocketByte ? EPlayerWeaponSockets::DawnSocket : EPlayerWeaponSockets::UpSocket;
}

bool UWeaponBase::IsWeaponInfoVisible()
{
	return bWeaponInfoVisible;
}

void UWeaponBase::GetSocketsInfo(TArray<FDamageEffects>& Info)
{
	if (UpSocket)
	{
		Info.Add(UpSocket->GetDamageSettings());
	}

	if (DawnSocket)
	{
		Info.Add(DawnSocket->GetDamageSettings());
	}
}

void UWeaponBase::RemoveWeapon()
{
	bWeaponEquipped = false;
	StopFire();
	SetWeaponInfoVisibility(false);
}

void UWeaponBase::EquipWeapon(const FWeaponEquipInfo& InWeaponInfo)
{
	bWeaponEquipped = true;
	SwitchSocket(InWeaponInfo.WeaponByteSocket);
	SetWeaponInfoVisibility(InWeaponInfo.bInfoVisible);
}

UWeaponBase::FWeaponEquipInfo UWeaponBase::GetWeaponEquipInfo() const
{
	FWeaponEquipInfo Result;
	Result.WeaponByteSocket = SocketByte;
	Result.bInfoVisible = bWeaponInfoVisible;

	return Result;
}

void UWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		ClearSockets();
	}
	Super::EndPlay(EndPlayReason);
}

void UWeaponBase::SpawnSockets(
	const FWeaponSocketData& InUpSocketSettings,
	const FWeaponSocketData& InDawnSocketSettings)
{
	FWeaponBaseLocal::FWeaponSocketSpawnContext SpawnContext;
	SpawnContext.WeaponBase = this;
	SpawnContext.AttachToComponent = WeaponBaseMesh;
	SpawnContext.Player = Player;

	if (InUpSocketSettings.IsValid())
	{
		DawnSocket = FWeaponBaseLocal::CreateWeaponSocket(
			SpawnContext,
			InDawnSocketSettings);
	}
	if (InDawnSocketSettings.IsValid())
	{
		UpSocket = FWeaponBaseLocal::CreateWeaponSocket(
			SpawnContext,
			InUpSocketSettings);
	}

	CurrentSocket = UpSocket;
}

void UWeaponBase::SwitchSocket(bool InSocketByte)
{
	if ( InSocketByte == SocketByte )
	{
		return;
	}
	if (CurrentSocket)
	{
		CurrentSocket->SetWeaponState(EWeaponSocketStates::DifferentWeapon);
		CurrentSocket->SetInfoVisibility(false);
	}
	
	SocketByte = InSocketByte;
	CurrentSocket = SocketByte ? DawnSocket : UpSocket;

	if (bWeaponInfoVisible)
	{
		CurrentSocket->SetInfoVisibility(true);
	}
}

void UWeaponBase::CreateVisualComponents(const FWeaponSettings& InWeaponSettings, USceneComponent* InAttachToComponent)
{
	WeaponBaseMesh = NewObject<USkeletalMeshComponent>(Player);
	WeaponBaseMesh->SetSkeletalMesh(InWeaponSettings.WeaponMesh);
	WeaponBaseMesh->AttachToComponent(
		InAttachToComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		InWeaponSettings.AttachSocketName);
	WeaponBaseMesh->RegisterComponent();
}

void UWeaponBase::ClearSockets()
{
	if (IsValid(DawnSocket))
	{
		DawnSocket->DestroyComponent();
	}

	if (IsValid(UpSocket))
	{
		UpSocket->DestroyComponent();
	}
}

void UWeaponBase::Fire()
{
	check(CurrentSocket);
	CurrentSocket->SetWeaponState(EWeaponSocketStates::Fire);
}

void UWeaponBase::StopFire()
{
	check(CurrentSocket);
	CurrentSocket->SetWeaponState(EWeaponSocketStates::NotFire);
}

void UWeaponBase::SwitchSocket()
{
	const bool NewSocketByte = !SocketByte;
	SwitchSocket(NewSocketByte);
}

void UWeaponBase::SocketMontagePlayed()
{
	if (Player)
	{
		Player->PlayAnimMontage(PlayerFireMontage);
	}
}

APlayerCharacter* UWeaponBase::GetPlayer() const
{
	return Player;
}

void UWeaponBase::SetPlayer(APlayerCharacter* InPlayer)
{
	check(InPlayer);
	Player = InPlayer;
}

void UWeaponBase::SetWeaponInfoVisibility(bool bVisible)
{
	bWeaponInfoVisible = bVisible;
	if (CurrentSocket)
	{
		CurrentSocket->SetInfoVisibility(bVisible);
	}
}
