// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/ResourcePickUps/PlayerResourcePickUpMagnetic.h"
#include "ArmourPickUp.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AArmourPickUp : public APlayerResourcePickUpMagnetic
{
	GENERATED_BODY()    
public:
    UFUNCTION(BlueprintCallable, Category="Resources")
    void SetArmourAmount(const int Armour);

    UFUNCTION(BlueprintCallable, Category="Resources")
    int GetArmourAmount() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
    int ArmourAmount = 2;
    
    virtual void TransferResources(APlayerCharacter* Player) override;
};
