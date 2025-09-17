// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/ResourcePickUps/PlayerResourcePickUpMagnetic.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

APlayerResourcePickUpMagnetic::APlayerResourcePickUpMagnetic()
{
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    PickUpMesh->SetSimulatePhysics(true);
}

void APlayerResourcePickUpMagnetic::OverlapCheckOnSpawn()
{
    Super::OverlapCheckOnSpawn();

    PickUpMesh->SetSimulatePhysics(false);
    PickUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ACharacter *Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    ProjectileMovementComponent->bIsHomingProjectile = true;
    ProjectileMovementComponent->HomingTargetComponent = (USceneComponent*)Character->GetCapsuleComponent();
    ProjectileMovementComponent->MaxSpeed = 5000.0f;
    ProjectileMovementComponent->InitialSpeed = 5000.0f;
    ProjectileMovementComponent->HomingAccelerationMagnitude = 30000.0f;
}
