// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/EnemyLookAtTargetAnimNotifyState.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "Enemies/AnimInstances/EnemyBasicAnimInstance.h"

void UEnemyLookAtTargetAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration)
{
    if (MeshComp != nullptr)
    {
        UEnemyBasicAnimInstance *AnimInstance = Cast<UEnemyBasicAnimInstance>(MeshComp->GetAnimInstance());
        if (AnimInstance)
        {
            AnimInstance->SetInterpSpeedCalculation(InterpSpeed);
            AnimInstance->SetTargetPitchCalculation(true);
        }
    }
}

void UEnemyLookAtTargetAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float FrameDeltaTime)
{
    EnemyRef = Cast<AEnemyParent>(MeshComp->GetOwner());
    
    if (EnemyRef)
    {
        EnemyRef->SetRotationLookAtTarget(InterpSpeed);
    }
}

void UEnemyLookAtTargetAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UEnemyBasicAnimInstance *AnimInstance = Cast<UEnemyBasicAnimInstance>(MeshComp->GetAnimInstance());

    if (AnimInstance)
    {
        AnimInstance->SetInterpSpeedCalculation(15.0);
        AnimInstance->SetTargetPitchCalculation(false);
    }
}
