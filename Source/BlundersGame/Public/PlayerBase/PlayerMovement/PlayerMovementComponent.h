// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBase/MovementManager.h"

#include "PlayerMovementComponent.generated.h"

class UMomentumAccumulatorComponent;
class UMovementManager;

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
    void Init(UMomentumAccumulatorComponent* InMomentumComp, UMovementManager* InMovementManager);

protected:
    virtual FVector ScaleInputAcceleration(const FVector& InputAcceleration) const override;
    virtual void PerformMovement(float DeltaTime) override;

private:
    UPROPERTY()
    UMomentumAccumulatorComponent* MomentumAccumulator;

    UPROPERTY()
    UMovementManager* MovementManager;
};
