// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/EnemyArealDamageAnimNotify.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemies/EnemyParent.h"
#include "Kismet/GameplayStatics.h"


void UEnemyArealDamageAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        AEnemyParent *Enemy = Cast<AEnemyParent>(MeshComp->GetOwner());
        if (Enemy)
        {
            UCharacterMovementComponent *MovementComponent = Cast<UCharacterMovementComponent>(Enemy->GetMovementComponent());
            MovementComponent->StopMovementImmediately();
        }

        FVector DamageOffsetLocation = FVector::ZeroVector;
        if (Enemy)
        {
            switch (OffsetDirection)
            {
            case EDamageOffsetDirection::Up:
                DamageOffsetLocation = Enemy->GetActorUpVector();
                break;
            case EDamageOffsetDirection::Forward:
                DamageOffsetLocation = Enemy->GetActorForwardVector();
                break;
            case EDamageOffsetDirection::Right:
                DamageOffsetLocation = Enemy->GetActorRightVector();
                break;
            }
        }

        TArray<AActor*> IgnoredActors;
        IgnoredActors.Add(Enemy);
        if (Enemy)
        {
            UGameplayStatics::ApplyRadialDamage(Enemy->GetWorld(),
                                                BaseDamage,
                                                Enemy->GetActorLocation() + DamageOffsetLocation * DamageOffset,
                                                DamageRadius,
                                                DamageType,
                                                IgnoredActors,
                                                Enemy,
                                                Enemy->GetController(),
                                                true);
        }
    }
}
