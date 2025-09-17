// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementStates/PlayerMovementState.h"
#include "PlayerFallDownStates.h"
#include "SpecialAttacksStates.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerAttackStateGround : public UPlayerMovementState
{
	GENERATED_BODY()


public:
    virtual void OnExecute() override;

private:
    UFUNCTION()
    void AttackEnd();
    
    UPROPERTY()
    APlayerController *PlayerController;
};



/**
 *
 */
UCLASS()
class BLUNDERSGAME_API UPlayerAttackStateAir : public UPlayerFallDownState
{
    GENERATED_BODY()


public:
    virtual void OnLanding() override;

private:
    UFUNCTION()
    void AttackEnd();
    
    UPROPERTY()
    APlayerController *PlayerController;

    float GravityScaleSave;

    UPROPERTY();
    class UCharacterMovementComponent *PlayerMovementComponent;
};
