// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/EnemyShotAnimNotify.h"
#include "Enemies/EnemyProjectiles/EnemyProjectile.h"

#include "Enemies/EnemyParent.h"
#include "Kismet/KismetMathLibrary.h"

UEnemyShotAnimNotify::UEnemyShotAnimNotify()
{
    ShotSocketName = "ShotSocket";
}

void UEnemyShotAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        AEnemyParent *EnemyRef = Cast<AEnemyParent>(MeshComp->GetOwner());

        if (EnemyRef)
        {           
            const FVector StartLocation = EnemyRef->GetActorLocation();
            const FVector EndLocation = EnemyRef->GetCurrentTarget()->GetActorLocation();

            FRotator ShotRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
            const FVector SpawnLocation = EnemyRef->GetMesh()->GetSocketLocation(ShotSocketName);

            ShotRotation.Yaw += FMath::RandRange(-SpreadAngle, SpreadAngle);
            ShotRotation.Pitch += FMath::RandRange(-SpreadAngle, SpreadAngle);
            
            AEnemyProjectile *SpawnedProjectile = EnemyRef->GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileType, SpawnLocation, ShotRotation);
            SpawnedProjectile->SetTarget(EnemyRef->GetCurrentTarget());
        }
    }
}
