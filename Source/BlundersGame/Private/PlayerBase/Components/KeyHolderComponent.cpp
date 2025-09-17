// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/KeyHolderComponent.h"
#include "BlundersGame/Public/PlayerBase/UI/KeysScreenWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/BlundersGameInstance.h"

UKeyHolderComponent::UKeyHolderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	KeysMap.Add(EKeyTypes::Red, 0);
	KeysMap.Add(EKeyTypes::Green, 0);
	KeysMap.Add(EKeyTypes::Blue, 0);
	KeysMap.Add(EKeyTypes::White, 0);

	
}

void UKeyHolderComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	LoadKeysFromGameInstance();
	
	if (KeysScreenWidgetType && !KeysScreenWidget)
	{
		KeysScreenWidget = CreateWidget<UKeysScreenWidget>(GetWorld(), KeysScreenWidgetType);
		if (KeysScreenWidget)
		{
			KeysScreenWidget->SetKeyHolder(this);
			KeysScreenWidget->AddToViewport(100);
			KeysScreenWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UKeyHolderComponent::ReceiveKey(const EKeyTypes& KeyType, const uint8& KeyAmount)
{
	uint8 *CurrentAmount = KeysMap.Find(KeyType);
	if (CurrentAmount)
	{
		KeysMap.Add(KeyType, (*CurrentAmount) + KeyAmount);
	}

	UploadKeysToGameInstance();
}

bool UKeyHolderComponent::RemoveKey(const EKeyTypes& KeyType)
{
	uint8 *CurrentAmount = KeysMap.Find(KeyType);
	if (CurrentAmount && *CurrentAmount > 0)
	{
		KeysMap.Add(KeyType, --(*CurrentAmount));
		UploadKeysToGameInstance();
		
		return true;
	}

	return false;
}

bool UKeyHolderComponent::CombineKeysToWhite()
{
	uint8 *RedKeys = KeysMap.Find(EKeyTypes::Red);
	uint8 *GreenKeys = KeysMap.Find(EKeyTypes::Green);
	uint8 *BlueKeys = KeysMap.Find(EKeyTypes::Blue);

	if (*RedKeys > 0 && *GreenKeys > 0 && *BlueKeys > 0)
	{
		RemoveKey(EKeyTypes::Red);
		RemoveKey(EKeyTypes::Green);
		RemoveKey(EKeyTypes::Blue);

		ReceiveKey(EKeyTypes::White, 1);

		UploadKeysToGameInstance();

		return true;
	}

	return false;
}

void UKeyHolderComponent::ToggleKeysScreenVisibility()
{
	if (IsKeyScreenVisible)
	{
		IsKeyScreenVisible = false;
		HideKeysScreen();
	} else
	{
		IsKeyScreenVisible = true;
		ShowKeysScreen();
	}
}

void UKeyHolderComponent::ShowKeysScreen()
{
	if (KeysScreenWidget)
	{
		APlayerController *Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Controller->SetInputMode(FInputModeUIOnly());
		Controller->bShowMouseCursor = true;

		KeysScreenWidget->UpdateInfo();
		KeysScreenWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UKeyHolderComponent::HideKeysScreen()
{
	if (KeysScreenWidget)
	{
		APlayerController *Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = false;

		KeysScreenWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UKeyHolderComponent::LoadKeysFromGameInstance()
{
	UBlundersGameInstance *GameInstance = Cast<UBlundersGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		KeysMap.Append(GameInstance->GetPlayerKeysMap());
	}
}

void UKeyHolderComponent::UploadKeysToGameInstance() const
{
	UBlundersGameInstance *GameInstance = Cast<UBlundersGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetPlayerKeysMap(KeysMap);
	}
}

