// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/GAS/Tasks/AbilityTask_JumpCorrection.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UAbilityTask_JumpCorrection::Activate()
{
    Super::Activate();
    if (ControlledCharacter)
    {
        ControlledCharacter->GetWorld()->GetTimerManager().SetTimer(TrajectoryCorrectionTimerHandle, this, &UAbilityTask_JumpCorrection::TrajectoryCorrection, 0.2, true);
        ControlledCharacter->LandedDelegate.AddDynamic(this, &UAbilityTask_JumpCorrection::LandedCallback);

        FVector LaunchVelocity;
        FVector StartLocation = ControlledCharacter->GetActorLocation();
        FVector EndLocation = ControlledCharacter->GetCurrentTarget()->GetActorLocation();
        TargetActor = ControlledCharacter->GetCurrentTarget();
        
        UGameplayStatics::SuggestProjectileVelocity_CustomArc(ControlledCharacter->GetWorld(), LaunchVelocity, StartLocation, EndLocation, 0.0f, 0.5f);
        ControlledCharacter->LaunchCharacter(LaunchVelocity, false, false);

        InitialVelocity = LaunchVelocity;
    }
}

UAbilityTask_JumpCorrection* UAbilityTask_JumpCorrection::JumpCorrection(UGameplayAbility* OwningAbility, ACharacter *Character, UAnimInstance *AnimInstance)
{
    UAbilityTask_JumpCorrection *AbilityTask = NewAbilityTask<UAbilityTask_JumpCorrection>(OwningAbility);
    if (AbilityTask)
    {
        AbilityTask->ControlledCharacter = Cast<AGASEnemyBase>(Character);
        AbilityTask->AnimInstance = AnimInstance;
        AbilityTask->MontageLength = AnimInstance->GetCurrentActiveMontage()->SequenceLength;
    }

    return AbilityTask;
}

void UAbilityTask_JumpCorrection::OnDestroy(bool bInOwnerFinished)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "TaskDestroyed");
    if (ControlledCharacter)
    {
        ControlledCharacter->GetWorld()->GetTimerManager().ClearTimer(TrajectoryCorrectionTimerHandle);
    }
    Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_JumpCorrection::TrajectoryCorrection()
{
    if (ControlledCharacter)
    {
        //Character Launching Towards Player
        const FVector CurrentTargetLocation = TargetActor->GetActorLocation() + TargetActor->GetVelocity() / 2.0f;
        FVector Acceleration = (CurrentTargetLocation - ControlledCharacter->GetActorLocation()).GetSafeNormal() * InitialVelocity.Size();
        Acceleration.Z = Acceleration.Z > 0.0f ? 0.0f : Acceleration.Z;
        ControlledCharacter->LaunchCharacter(Acceleration * 0.2f, false, false);
        
        //Anim montage correction
        UAnimMontage *ActiveMontage = AnimInstance->GetCurrentActiveMontage();
        const float MontageTime = MontageLength - AnimInstance->Montage_GetPosition(ActiveMontage);
        const float JumpTime = FVector(CurrentTargetLocation - ControlledCharacter->GetActorLocation()).Size() / ControlledCharacter->GetVelocity().Size();

        // @TODO Figure Out How To Set Montage PlayRate Correctly. Alternative: Make Montages With Looping Sections
        const float MontageCorrectionCoefficient = FMath::Clamp(JumpTime / MontageTime, 1.0f, 40.0f);
        AnimInstance->Montage_SetPlayRate(ActiveMontage, MontageCorrectionCoefficient);

        //Character Rotation
        FRotator TargetRotation = ControlledCharacter->GetActorRotation();
        TargetRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(ControlledCharacter->GetActorLocation(), CurrentTargetLocation).Yaw;
        ControlledCharacter->SetActorRotation(TargetRotation);
    }
}

void UAbilityTask_JumpCorrection::LandedCallback(const FHitResult& Hit)
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {              
        WaitOnLanded.Broadcast(Hit.Location);
        ControlledCharacter->LandedDelegate.RemoveAll(this);

        EndTask();
    }
}
