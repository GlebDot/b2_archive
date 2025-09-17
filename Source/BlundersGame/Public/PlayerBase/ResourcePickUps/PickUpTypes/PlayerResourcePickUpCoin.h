// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/ResourcePickUps/PlayerResourcePickUpParent.h"
#include "PlayerResourcePickUpCoin.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API APlayerResourcePickUpCoin : public APlayerResourcePickUpParent
{
	GENERATED_BODY()

protected:
	virtual void TransferResources(APlayerCharacter* Player) override;
	
};
