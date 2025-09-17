// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/EnemyMountAttackAnimNotifyState.h"

#include "DrawDebugHelpers.h"
#include "BlundersGame/Public/Enemies/AnimInstances/EnemyMountAnimInstance.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetMathLibrary.h"


UEnemyMountAttackAnimNotifyState::UEnemyMountAttackAnimNotifyState()
{
    QueryParams = FCollisionQueryParams::DefaultQueryParam;
    LaserShape.SetSphere(10);
    ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
    ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
    ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
}

void UEnemyMountAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float FrameDeltaTime)
{
    
    UEnemyMountAnimInstance *AnimInstance = Cast<UEnemyMountAnimInstance>(MeshComp->GetAnimInstance());
    
    if (AnimInstance)
    {        
        const FVector EnemySocketLocation = MeshComp->GetSocketLocation(ShotSocket);
        const FRotator EnemySocketRotation = AnimInstance->GetOwningActor()->GetActorRotation();
        const FVector TargetLocation = AnimInstance->GetCurrentEnemyTarget()->GetActorLocation();
        const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(EnemySocketLocation, TargetLocation);
        const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotation, EnemySocketRotation);
        
        AnimInstance->SetMountPitch(FMath::FInterpTo(AnimInstance->GetMountPitch(), DeltaRotation.Pitch, MeshComp->GetWorld()->GetDeltaSeconds(), InterpolationSpeed));
        AnimInstance->SetMountYaw(FMath::FInterpTo(AnimInstance->GetMountYaw(), DeltaRotation.Yaw, MeshComp->GetWorld()->GetDeltaSeconds(), InterpolationSpeed));

        switch (AttackType)
        {
            case EAttackType::Gatling:
                break;
            case EAttackType::Laser:
                LaserAttack(MeshComp->GetWorld(), EnemySocketLocation, MeshComp->GetSocketRotation(ShotSocket));
                break;
            default:
                break;
        }
    }
}

void UEnemyMountAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration)
{
    if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, MeshComp->GetOwner()->GetName());
        UEnemyMountAnimInstance *AnimInstance = Cast<UEnemyMountAnimInstance>(MeshComp->GetAnimInstance());
        
        if (AnimInstance)
        {
            AnimInstance->SetIsMount(true);
            const FVector EnemySocketLocation = MeshComp->GetSocketLocation(ShotSocket);
            const FRotator EnemySocketRotation = MeshComp->GetSocketRotation(ShotSocket);
            
            const FVector TargetLocation = AnimInstance->GetCurrentEnemyTarget()->GetActorLocation();
            const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(EnemySocketLocation, TargetLocation);
            
            const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotation, EnemySocketRotation);
            AnimInstance->SetSocketRotation(EnemySocketRotation);

            AnimInstance->SetMountPitch(FMath::FInterpTo(AnimInstance->GetMountPitch(), DeltaRotation.Pitch, MeshComp->GetWorld()->GetDeltaSeconds(), InterpolationSpeed));
            AnimInstance->SetMountYaw(FMath::FInterpTo(AnimInstance->GetMountYaw(), DeltaRotation.Yaw, MeshComp->GetWorld()->GetDeltaSeconds(), InterpolationSpeed));
        }
    }
}

void UEnemyMountAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UEnemyMountAnimInstance *AnimInstance = Cast<UEnemyMountAnimInstance>(MeshComp->GetAnimInstance());
    if (AnimInstance)
    {
        AnimInstance->SetIsMount(false);
        AnimInstance->SetMountPitch(0.0);
        AnimInstance->SetMountYaw(0.0);
    }
}

void UEnemyMountAttackAnimNotifyState::LaserAttack(const UWorld *World, const FVector& StartLocation, const FRotator& SocketRotation)
{
    FVector EndLocation = StartLocation + UKismetMathLibrary::GetForwardVector(SocketRotation) * TraceDistance;

    FHitResult HitInfo;
    const bool IsHit = World->SweepSingleByObjectType(HitInfo, StartLocation, EndLocation, FQuat::Identity, ObjectParams, LaserShape, QueryParams);
    if (IsHit)
    {
        UGameplayStatics::ApplyDamage(HitInfo.GetActor(), AttackDamage, nullptr, nullptr, DamageType);

        EndLocation = HitInfo.Location;
    }
    
    DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 0.0, 0, 10);
}
