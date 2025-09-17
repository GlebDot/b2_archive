// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/EnemyBurstShotAnimNotify.h"
#include "Enemies/EnemyParent.h"
#include "Enemies/EnemyProjectiles/EnemyProjectile.h"
#include "Kismet/KismetMathLibrary.h"


void UEnemyBurstShotAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        AEnemyParent *EnemyRef = Cast<AEnemyParent>(MeshComp->GetOwner());

        if (EnemyRef)
        {           
            const FVector StartLocation = EnemyRef->GetActorLocation();
            const FVector EndLocation = EnemyRef->GetCurrentTarget()->GetActorLocation();
            const FVector ShotDirection = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation).Vector();

            const FVector SpawnLocation = MeshComp->GetSocketLocation(ShotSocketName);
            const int ShotsNumber = UKismetMathLibrary::RandomIntegerInRange(MinMaxProjectiles.X, MinMaxProjectiles.Y);

            for (int i = 0; i < ShotsNumber;  i++)
            {
                const FRotator SpawnRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDirection, SpreadAngle).Rotation();
                AEnemyProjectile *Projectile = EnemyRef->GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileType, SpawnLocation, SpawnRotation);
                Projectile->SetTarget(EnemyRef->GetCurrentTarget());
            }
        }
    }
}
