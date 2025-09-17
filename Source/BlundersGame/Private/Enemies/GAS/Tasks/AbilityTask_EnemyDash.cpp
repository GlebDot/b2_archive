// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/GAS/Tasks/AbilityTask_EnemyDash.h"
#include "BlundersGame/Public/Enemies/GAS/GASEnemyBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAbilityTask_EnemyDash::Activate()
{
    Super::Activate();

    if (EnemyOverlapComponent)
    {
        EnemyOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &UAbilityTask_EnemyDash::OverlapCallback);
    }

    BrakingFrictionSave = EnemyMovementComponent->BrakingFriction;
    BrakingFrictionFactorSave = EnemyMovementComponent->BrakingFrictionFactor;
    GroundFrictionSave = EnemyMovementComponent->GroundFriction;
    EnemyMovementComponent->BrakingFriction = 0.0f;
    EnemyMovementComponent->GroundFriction = 0.0f;
    EnemyMovementComponent->BrakingFriction = 0.0f;
    
    const FVector TargetLocation = Enemy->GetCurrentTarget()->GetActorLocation();
    const FVector StartLocation = Enemy->GetActorLocation();

    FRotator LaunchRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
    const FVector LaunchVelocity = LaunchRotation.Vector() * DashSpeed;

    const float DashTime = FVector::Distance(TargetLocation, StartLocation) / DashSpeed;
    
    Enemy->LaunchCharacter(LaunchVelocity, true, false);

    LaunchRotation.Pitch = 0.0f;
    Enemy->SetActorRotation(LaunchRotation);

    Enemy->GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAbilityTask_EnemyDash::DashEnd, DashTime, false);

    //Adjust Montage Length
    UAnimMontage *CurrentMontage = AnimInstance->GetCurrentActiveMontage();
    const float MontageCorrectionCoefficient = DashTime / (CurrentMontage->SequenceLength - AnimInstance->Montage_GetPosition(CurrentMontage));
    AnimInstance->Montage_SetPlayRate(CurrentMontage, MontageCorrectionCoefficient);
    
}

UAbilityTask_EnemyDash* UAbilityTask_EnemyDash::EnemyDash(UGameplayAbility* OwningAbility, ACharacter* Character,
                                                          UPrimitiveComponent* Component, UAnimInstance *AnimInstance, float DashSpeed)
{
    UAbilityTask_EnemyDash *DashAbility = NewAbilityTask<UAbilityTask_EnemyDash>(OwningAbility);
    DashAbility->Enemy = Cast<AGASEnemyBase>(Character);
    DashAbility->EnemyOverlapComponent = Component;
    DashAbility->EnemyMovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
    DashAbility->AnimInstance = AnimInstance;
    DashAbility->DashSpeed = DashSpeed;

    return DashAbility;
}

void UAbilityTask_EnemyDash::OnDestroy(bool bInOwnerFinished)
{
    if (EnemyOverlapComponent)
    {
        EnemyOverlapComponent->OnComponentBeginOverlap.RemoveAll(this);
    }

    Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_EnemyDash::OverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
    if (Player)
    {
        if(ShouldBroadcastAbilityTaskDelegates())
        {
            OnDashOverlap.Broadcast(Player);
        }

        DashEnd();
    }
}

void UAbilityTask_EnemyDash::DashEnd()
{
    EnemyMovementComponent->StopMovementImmediately();
    EnemyMovementComponent->BrakingFriction = BrakingFrictionFactorSave;
    EnemyMovementComponent->GroundFriction = GroundFrictionSave;
    EnemyMovementComponent->BrakingFriction = BrakingFrictionSave;

    Enemy->GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);

    EndTask();
}

