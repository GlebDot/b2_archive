// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase/PlayerMovement/PlayerMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"


void UPlayerMovementComponent::Init(UMomentumAccumulatorComponent* InMomentumComp, UMovementManager* InMovementManager)
{
    MomentumAccumulator = InMomentumComp;
    MovementManager = InMovementManager;
}

FVector UPlayerMovementComponent::ScaleInputAcceleration(const FVector& InputAcceleration) const
{
    const FVector ClampedInputAcceleration = InputAcceleration.GetClampedToMaxSize(1.0f);
    const FVector ClampedMomentum = MomentumAccumulator->GetMomentumScaled().GetClampedToMaxSize(1.0f);
    const float MomentumAccelerationMultiplier = MomentumAccumulator->GetMomentumMagnitudeRatio();     
    return GetMaxAcceleration() * MomentumAccelerationMultiplier *
        (ClampedMomentum + ClampedInputAcceleration).GetClampedToMaxSize(1.0f);
}

void UPlayerMovementComponent::PerformMovement(float DeltaTime)
{
    // So i will do all MY computation BEFORE the actual movement.

    // My computations ...
    MomentumAccumulator->AccumulateMomentum(GetLastInputVector().GetSafeNormal2D() * GetMaxSpeed(), DeltaTime);

    // Actual movement 
    Super::PerformMovement(DeltaTime);

    if (IsValid(UpdatedComponent))
    {
        const FVector VelocityLineStart = UpdatedComponent->GetComponentLocation();
        const FVector MomentumLineStart = VelocityLineStart + FVector{0.0f, 0.0f, 100.0f};
        const FVector VelocityLineEnd = VelocityLineStart + Velocity;
        const FVector MomentumLineEnd = MomentumLineStart + MomentumAccumulator->GetMomentum();
        DrawDebugLine(GetWorld(), VelocityLineStart, VelocityLineEnd, FColor::Red, false, 0, 0, 3);
        DrawDebugLine(GetWorld(), MomentumLineStart, MomentumLineEnd, FColor::Green, false, 0, 0, 3);
    }
}
