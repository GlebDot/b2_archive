// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlundersGame/Public/PlayerMovementStates/PlayerMovementState.h"
#include "PlayerDashState.generated.h"
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerDashState : public UPlayerMovementState
{
	GENERATED_BODY()

public:
    UPlayerDashState();
    virtual void OnExecute() override;
    virtual void OnChange() override;
protected:
    //Timer Handle for setting timer
    FTimerHandle TH_Dash;
    //Timer Delegate for binding the function to it
    FTimerDelegate TD_Dash;

    UPROPERTY()
    float BrakingFrictionSave;
    
    UPROPERTY()
    float BrakingFrictionFactorSave;
};


UCLASS()
class BLUNDERSGAME_API UPlayerDashStateVertical : public UPlayerDashState
{
    GENERATED_BODY()
public:
    UPlayerDashStateVertical();
    virtual void OnExecute() override;
};

UCLASS()
class BLUNDERSGAME_API UPlayerDashStateLong : public UPlayerDashState
{
    GENERATED_BODY()
public:
    UPlayerDashStateLong();
    virtual void OnExecute() override;
protected:
    UPROPERTY()
    APlayerController *Controller;

    UFUNCTION()
    void LongDashStop();
};
