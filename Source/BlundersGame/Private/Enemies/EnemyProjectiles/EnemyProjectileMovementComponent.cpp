// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemyProjectiles/EnemyProjectileMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"


FVector UEnemyProjectileMovementComponent::ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const
{
    FVector HomingAcceleration = ((HomingTargetComponent->GetComponentLocation() - UpdatedComponent->GetComponentLocation()).GetSafeNormal() * HomingAccelerationMagnitude);
    const FVector DesiredVelocity = InVelocity + HomingAcceleration * DeltaTime; 
    const float DotProduct = FVector::DotProduct(HomingAcceleration.GetSafeNormal(), DesiredVelocity.GetSafeNormal());

    if (DotProduct < 0.0f)
    {
        HomingAcceleration = FVector::ZeroVector;
        if (OnProjectileFlyOverTargetDelegate.IsBound())
        {
            OnProjectileFlyOverTargetDelegate.Execute();
        }
    }

    return HomingAcceleration;
}

void UEnemyProjectileMovementComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    if (!OnProjectileFlyOverTargetDelegate.IsBound())
    {
        OnProjectileFlyOverTargetDelegate.BindUFunction(this, FName("OnProjectileFlyOver"));
    }
}

void UEnemyProjectileMovementComponent::OnProjectileFlyOver()
{
    bIsHomingProjectile = false;
    OnProjectileFlyOverTargetDelegate.Unbind();
}
