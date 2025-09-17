// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/ResourcePickUps/PlayerResourcePickUpParent.h"
#include "PlayerResourcePickUpMagnetic.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API APlayerResourcePickUpMagnetic : public APlayerResourcePickUpParent
{
	GENERATED_BODY()

public:
	APlayerResourcePickUpMagnetic();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UProjectileMovementComponent *ProjectileMovementComponent;

protected:
	virtual void OverlapCheckOnSpawn() override;
	
};
