// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/JumpAttackAnimNotify.h"

#include "Enemies/EnemyParent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UJumpAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        AEnemyParent *Enemy = Cast<AEnemyParent>(MeshComp->GetOwner());
        if (Enemy)
        {
            switch (AttackType)
            {
                case EJumpAttackType::ProjectileType:
                    ProjectileLaunch(Enemy);
                    break;
                case EJumpAttackType::StraightLaunch:
                    StraightLaunch(Enemy);
                    break;
            }                     
        }
    }
}

void UJumpAttackAnimNotify::ProjectileLaunch(AEnemyParent* Enemy)
{
    const float Distance = FVector(Enemy->GetActorLocation() - Enemy->GetCurrentTarget()->GetActorLocation()).Size();
    float Alpha = FMath::Clamp(Distance / 3000.0, 0.0, 1.0);
    Alpha = 1 - (1 - Alpha) * (1 - Alpha);
    const float ArcAngle = FMath::Lerp(0.5, 0.9, Alpha);
    FVector LaunchVelocity;
    UGameplayStatics::SuggestProjectileVelocity_CustomArc(Enemy->GetWorld(), LaunchVelocity, Enemy->GetActorLocation(), Enemy->GetCurrentTarget()->GetActorLocation(), GravityOverride, ArcAngle);
    Enemy->LaunchCharacter(LaunchVelocity, true, true);
}

void UJumpAttackAnimNotify::StraightLaunch(AEnemyParent* Enemy)
{
    FVector StartLocation = Enemy->GetActorLocation();
    StartLocation.Z -= Enemy->GetSimpleCollisionHalfHeight();
    FVector LaunchVelocity = UKismetMathLibrary::FindLookAtRotation(StartLocation, Enemy->GetCurrentTarget()->GetActorLocation()).Vector();
    LaunchVelocity *= LaunchForce;

    Enemy->LaunchCharacter(LaunchVelocity, true, true);
}
