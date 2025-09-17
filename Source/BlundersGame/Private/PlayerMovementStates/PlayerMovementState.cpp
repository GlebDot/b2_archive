// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementStates/PlayerMovementState.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "BlundersGame/Public/PlayerBase/Components/AdvancedActionsComponent.h"
#include "PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"

void UPlayerMovementState::OnExecute()
{
    if (MomentumComponent)
    {
        MomentumComponent->ChangeMagnitudePreserveDirection(Params.MagnitudeChangePercent);
    }
}

const FMovementStateData& UPlayerMovementState::GetStateParams() const
{
    return Params;
}

APlayerCharacter* UPlayerMovementState::GetPlayerRef() const
{
    return Player;
}

bool UPlayerMovementState::GetIsPermanent() const
{
    return bPermanent;
}

float UPlayerMovementState::CalculateMomentumAdjustedVelocity() const
{
    return Params.StateVelocity * (1.0f + MomentumComponent->GetAdditionalMagnitudeRatioClamped() * Params.MagnitudeEffectFactor);
}

void UPlayerMovementState::NotifyPlayerStateChange() const
{
    if (Player)
    {
        if (Player->OnPlayerMovementStateChanged.IsBound())
        {
            Player->OnPlayerMovementStateChanged.Execute(StateType);
        }
    }
}

void UPlayerMovementState::Init(
    UMomentumAccumulatorComponent* InMomentum,
    UMovementManager* InMovementManager, 
    APlayerCharacter* InPlayer,
    UPlayerMovementComponent* InMovementComponent)
{
    MomentumComponent = InMomentum;
    MovementManager = InMovementManager;
    Player = InPlayer;
    MovementComponent = InMovementComponent;
}

void UPlayerMovementState::SetParameters(const FMovementStateData& InStateData, float InStateMultiplier)
{
    StateMultiplier = InStateMultiplier;
    Params = InStateData;
    Params *= StateMultiplier;
}
