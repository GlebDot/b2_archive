// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/ResourcePickUps/PlayerResourcePickUpMagnetic.h"
#include "GeneralPickUp.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AGeneralPickUp : public APlayerResourcePickUpMagnetic
{
	GENERATED_BODY()
public:
    AGeneralPickUp();

protected:
    virtual void TransferResources(APlayerCharacter* Player) override;

};
