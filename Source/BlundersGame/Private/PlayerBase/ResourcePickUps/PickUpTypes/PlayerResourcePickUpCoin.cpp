// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/ResourcePickUps/PickUpTypes/PlayerResourcePickUpCoin.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerBase/BlundersGameInstance.h"

void APlayerResourcePickUpCoin::TransferResources(APlayerCharacter* Player)
{
    UBlundersGameInstance *GameInstance = Cast<UBlundersGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        GameInstance->IncreaseCoins(1);
    }
}
