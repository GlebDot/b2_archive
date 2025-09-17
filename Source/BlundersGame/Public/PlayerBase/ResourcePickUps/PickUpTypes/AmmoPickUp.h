// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerBase/ResourcePickUps/PlayerResourcePickUpMagnetic.h"
#include "AmmoPickUp.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AAmmoPickUp : public APlayerResourcePickUpMagnetic
{
	GENERATED_BODY()

protected:
    virtual void TransferResources(APlayerCharacter* Player) override;
    
private:
    UPROPERTY()
    int AmmoToRestore = 10;
};
