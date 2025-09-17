// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlundersGame/Public/PlayerBase/Components/AdvancedActionsComponent.h"
#include "PlayerMovementState.generated.h"

class APlayerCharacter;
class UMomentumAccumulatorComponent;
class UMovementManager;
class UPlayerMovementComponent;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerMovementState : public UObject
{
    GENERATED_BODY()

public:
    void Init(
        UMomentumAccumulatorComponent* InMomentum,
        UMovementManager* InMovementManager,
        APlayerCharacter* InPlayer,
        UPlayerMovementComponent* InMovementComponent);

    void SetParameters(const FMovementStateData& InStateData, float InStateMultiplier = 1.0f);

    virtual void OnExecute();
    virtual void OnColliding() {};
    virtual void OnChange() {};
    virtual void OnLanding() {};
    virtual void SetScalarParameter(float Value) {};
    virtual void SetVectorParameter(const FVector &Value) {};

    const FMovementStateData& GetStateParams() const;
    APlayerCharacter* GetPlayerRef() const;
    bool GetIsPermanent() const;
protected:
    UPROPERTY()
    APlayerCharacter *Player;

    UPROPERTY()
    UPlayerMovementComponent *MovementComponent;

    FMovementStateData Params {};
    float StateMultiplier = 1.0f;

    EMovementStateSubtype StateType;
    
    UPROPERTY()
    bool bPermanent = false;

    UPROPERTY()
    UMomentumAccumulatorComponent* MomentumComponent;

    UPROPERTY()
    UMovementManager* MovementManager;

    float CalculateMomentumAdjustedVelocity() const;
    void NotifyPlayerStateChange() const;
};
