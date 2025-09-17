// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemyProjectiles/LockOnEnemyProjectile.h"
#include "BlundersGame/Public/Enemies/EnemyProjectiles/EnemyProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALockOnEnemyProjectile::ALockOnEnemyProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    ProjectileMovementComponent->MaxSpeed = 500.0f;
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
}

void ALockOnEnemyProjectile::BeginPlay()
{
    Super::BeginPlay();

    
    GetWorld()->GetTimerManager().SetTimer(TH_Activation, this, &ALockOnEnemyProjectile::ActivateProjectile, ActivationTime, false);
}

void ALockOnEnemyProjectile::BeginDestroy()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TH_Activation);
    }
    Super::BeginDestroy();
}

void ALockOnEnemyProjectile::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!ProjectileMovementComponent->bIsHomingProjectile)
    {
        ProjectileMovementComponent->MaxSpeed = FMath::FInterpTo(ProjectileMovementComponent->MaxSpeed, 0.0f, DeltaSeconds, ProjectileSpeedDecreasingRate);
    } else
    {
        PrimaryActorTick.bCanEverTick = false;
    }
}

void ALockOnEnemyProjectile::ActivateProjectile()
{
    GetWorld()->GetTimerManager().ClearTimer(TH_Activation);
    
    if (ProjectileTarget)
    {
        const FVector VelocityDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ProjectileTarget->GetActorLocation()).Vector();
        ProjectileMovementComponent->Velocity = VelocityDirection;
        ProjectileMovementComponent->MaxSpeed = SpeedAfterActivation;
        ProjectileMovementComponent->InitialSpeed = SpeedAfterActivation / 1.5f;

        ProjectileMovementComponent->bIsHomingProjectile = true;
        ProjectileMovementComponent->HomingTargetComponent = ProjectileTarget->GetRootComponent();
        ProjectileMovementComponent->HomingAccelerationMagnitude = SpeedAfterActivation * 4.0f;
    } else
    {
        Destroy();
    }
}
