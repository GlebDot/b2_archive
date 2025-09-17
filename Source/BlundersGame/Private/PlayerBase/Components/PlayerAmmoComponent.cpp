// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/PlayerAmmoComponent.h"


#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/UI/PlayerHUD.h"

// Sets default values for this component's properties
UPlayerAmmoComponent::UPlayerAmmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUDRef = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
}

void UPlayerAmmoComponent::AddLightAmmo(const int Ammo)
{
	CurrentLightAmmo += Ammo;
	CurrentLightAmmo = FMath::Clamp(CurrentLightAmmo, 0, MaxLightAmmo);
	APlayerHUD *PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (PlayerHUD)
	{
		PlayerHUD->PlayerAmmoChanged(EAmmoRequestType::Light, CurrentLightAmmo * 1.0f / MaxLightAmmo);
	}
}

void UPlayerAmmoComponent::AddHeavyAmmo(const int Ammo)
{
	CurrentHeavyAmmo += Ammo;
	CurrentHeavyAmmo = FMath::Clamp(CurrentHeavyAmmo, 0, MaxHeavyAmmo);
	APlayerHUD *PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (PlayerHUD)
	{
		PlayerHUD->PlayerAmmoChanged(EAmmoRequestType::Heavy, CurrentHeavyAmmo * 1.0f / MaxHeavyAmmo);
	}
}

void UPlayerAmmoComponent::AddCigarette(const int Ammo)
{
	CurrentCigarettesCount += Ammo;
	CurrentCigarettesCount = FMath::Clamp(CurrentCigarettesCount, 0, MaxCigarettesCount);
}

bool UPlayerAmmoComponent::AmmoRequestCheck(const EAmmoRequestType RequestType, const int RequestAmount)
{
	switch (RequestType)
	{
		case EAmmoRequestType::Light:
			if (RequestAmount <= CurrentLightAmmo)
			{
				return true;
			}
			return false;
		case EAmmoRequestType::Heavy:
			if (RequestAmount <= CurrentHeavyAmmo)
			{
				return true;
			}
			return false;
		case EAmmoRequestType::Both:
			if (RequestAmount <= CurrentHeavyAmmo && RequestAmount <= CurrentLightAmmo)
			{
				return true;
			}
			return false;
		case EAmmoRequestType::Cigarette:
			if (RequestAmount <= CurrentCigarettesCount)
			{
				return true;
			}
			return false;
	}

	return true;
}

void UPlayerAmmoComponent::ReduceAmmo(const EAmmoRequestType AmmoType, const int AmmoAmount)
{
	switch (AmmoType)
	{
		case EAmmoRequestType::Light:
			CurrentLightAmmo -= AmmoAmount;
			CurrentLightAmmo = FMath::Clamp(CurrentLightAmmo, 0, MaxLightAmmo);
			PlayerHUDRef->PlayerAmmoChanged(EAmmoRequestType::Light, CurrentLightAmmo * 1.0 / MaxLightAmmo);
			break;
		case EAmmoRequestType::Heavy:
			CurrentHeavyAmmo -= AmmoAmount;
			CurrentHeavyAmmo = FMath::Clamp(CurrentHeavyAmmo, 0, MaxHeavyAmmo);
			PlayerHUDRef->PlayerAmmoChanged(EAmmoRequestType::Heavy, CurrentHeavyAmmo * 1.0 / MaxHeavyAmmo);
			break;
		case EAmmoRequestType::Both:
			CurrentHeavyAmmo -= AmmoAmount;
			CurrentLightAmmo -= AmmoAmount;
			CurrentHeavyAmmo = FMath::Clamp(CurrentHeavyAmmo, 0, MaxHeavyAmmo);
			CurrentLightAmmo = FMath::Clamp(CurrentLightAmmo, 0, MaxLightAmmo);
			PlayerHUDRef->PlayerAmmoChanged(EAmmoRequestType::Light, CurrentLightAmmo * 1.0 / MaxLightAmmo);
			PlayerHUDRef->PlayerAmmoChanged(EAmmoRequestType::Heavy, CurrentHeavyAmmo * 1.0 / MaxHeavyAmmo);
			break;
		case EAmmoRequestType::Cigarette:
			CurrentCigarettesCount -= AmmoAmount;
			CurrentCigarettesCount = FMath::Clamp(CurrentCigarettesCount, 0, MaxCigarettesCount);
			break;
	}
}

void UPlayerAmmoComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	CurrentHeavyAmmo = MaxHeavyAmmo;
	CurrentLightAmmo = MaxLightAmmo;
	CurrentCigarettesCount = 0;
}

