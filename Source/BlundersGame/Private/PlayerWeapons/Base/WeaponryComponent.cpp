
#include "PlayerWeapons/Base/WeaponryComponent.h"

#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerBase/PlayerCharacter.h"
#include "PlayerWeapons/Base/WeaponryConfig.h"

namespace FWeaponryLocal
{
	struct FWeaponSpawnContext
	{
		TWeakObjectPtr<USceneComponent> AttachToComponent {};
		TWeakObjectPtr<APlayerCharacter> PlayerCharacter {};

		bool IsValid() const
		{
			return AttachToComponent.IsValid() && PlayerCharacter.IsValid();
		}
	};
	
	UWeaponBase* CreateWeapon(const FWeaponSettings& InWeaponSettings, const FWeaponSpawnContext& Context)
	{
		check(Context.IsValid());

		UWeaponBase* Weapon = NewObject<UWeaponBase>(Context.PlayerCharacter.Get(), InWeaponSettings.Weapon);
		Weapon->RegisterComponent();
		Weapon->SetPlayer(Context.PlayerCharacter.Get());
		Weapon->InitConstructor(InWeaponSettings, Context.AttachToComponent.Get());

		return Weapon;
	}
}

void UWeaponryComponent::Init(APlayerCharacter* InPlayer)
{
	check(InPlayer);
	PlayerCharacter = InPlayer;
	bInCombatState = PlayerCharacter->bInCombatState;
	PlayerCharacter->OnPlayerSwitchedCombatState.AddUObject(this, &UWeaponryComponent::PlayerSwitchedCombatState);

	ClearWeapons();
	SpawnWeapons();
	SetupWeapons();
}

void UWeaponryComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &UWeaponryComponent::FirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &UWeaponryComponent::FireReleased);
	PlayerInputComponent->BindAction("SocketSwitch", IE_Pressed, this, &UWeaponryComponent::SwitchSocketPressed);
	PlayerInputComponent->BindAction("SocketSwitch", IE_Released, this, &UWeaponryComponent::SwitchSocketPressed);
	PlayerInputComponent->BindAction("WeaponSwitch1", IE_Pressed, this, &UWeaponryComponent::WeaponSwitch1Pressed);
	PlayerInputComponent->BindAction("WeaponSwitch2", IE_Pressed, this, &UWeaponryComponent::WeaponSwitch2Pressed);
	PlayerInputComponent->BindAction("WeaponSwitch3", IE_Pressed, this, &UWeaponryComponent::WeaponSwitch3Pressed);
}

void UWeaponryComponent::SpawnWeapons()
{
	check(WeaponryConfig);
	FWeaponryLocal::FWeaponSpawnContext SpawnContext;
	SpawnContext.PlayerCharacter = PlayerCharacter;
	SpawnContext.AttachToComponent = PlayerCharacter->GetMesh();
	for (const FWeaponSettings& WeaponSettings : WeaponryConfig->WeaponSettings)
	{
		if (WeaponSettings.IsValid())
		{
			Weapons.Add(FWeaponryLocal::CreateWeapon(WeaponSettings, SpawnContext));
		}
	}
}

void UWeaponryComponent::SetupWeapons()
{
	check(Weapons.Num() > 0)
	CurrentWeapon = Weapons[0];
	check(CurrentWeapon);
	UWeaponBase::FWeaponEquipInfo WeaponInfo;
	WeaponInfo.bInfoVisible = PlayerCharacter->bInCombatState;
	WeaponInfo.WeaponByteSocket = 0;
	CurrentWeapon->EquipWeapon(WeaponInfo);
}

void UWeaponryComponent::FirePressed()
{
	check(CurrentWeapon);
	if (PlayerCharacter->bInCombatState)
	{
		CurrentWeapon->Fire();
	}
}

void UWeaponryComponent::FireReleased()
{
	check(CurrentWeapon);
	if (PlayerCharacter->bInCombatState)
	{
		CurrentWeapon->StopFire();
	}
}

void UWeaponryComponent::SwitchSocketPressed()
{
	check(CurrentWeapon);
	CurrentWeapon->SwitchSocket();
}

void UWeaponryComponent::SwitchWeapon(EWeaponIndex WeaponIndex)
{
	const uint8 Index = static_cast<uint8>(WeaponIndex);
	if ( Index >= Weapons.Num() )
	{
		return;
	}
	UWeaponBase* NewWeapon = Weapons[static_cast<uint8>(WeaponIndex)];
	if (CurrentWeapon != NewWeapon && IsValid(CurrentWeapon))
	{
		const UWeaponBase::FWeaponEquipInfo WeaponInfo = CurrentWeapon->GetWeaponEquipInfo();
		CurrentWeapon->RemoveWeapon();
		CurrentWeapon = NewWeapon;
		CurrentWeapon->EquipWeapon(WeaponInfo);
	}
}


void UWeaponryComponent::WeaponSwitch1Pressed()
{
	SwitchWeapon(EWeaponIndex::Idx1);
}

void UWeaponryComponent::WeaponSwitch2Pressed()
{
	SwitchWeapon(EWeaponIndex::Idx2);
}

void UWeaponryComponent::WeaponSwitch3Pressed()
{
	SwitchWeapon(EWeaponIndex::Idx3);
}

void UWeaponryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		PlayerCharacter->OnPlayerSwitchedCombatState.RemoveAll(this);
		ClearWeapons();
	}
	Super::EndPlay(EndPlayReason);
}

void UWeaponryComponent::PlayerSwitchedCombatState(bool bInState)
{
	bInCombatState = bInState;

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->SetWeaponInfoVisibility(bInCombatState);
	}
}

void UWeaponryComponent::ClearWeapons()
{
	for (UWeaponBase* WeaponBase : Weapons)
	{
		if (IsValid(WeaponBase))
		{
			WeaponBase->DestroyComponent();
		}
	}
	Weapons.Reset();
}
