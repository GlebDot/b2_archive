// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/ContinuousAttackBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlundersGame/Public/Enemies/AnimInstances/EnemyMountAnimInstance.h"

UContinuousAttackBTTaskNode::UContinuousAttackBTTaskNode()
{
    NodeName = "Continuous Attack";
    bNotifyTick = true;
    bCreateNodeInstance = true;
    ForceInstancing(true);
}

EBTNodeResult::Type UContinuousAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());

    if (Controller)
    {
        if (CanBeAborted)
        {
            AllowTick = true;
        }
        
        UBlackboardComponent *BlackboardComponent = Controller->GetBlackboardComponent();

        AActor *AttackTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(AttackTargetKey.SelectedKeyName));
        Controller->GetEnemyRef()->SetCurrentTarget(AttackTarget);
        
        AnimInstance = Cast<UEnemyMountAnimInstance>(Controller->GetEnemyRef()->GetMesh()->GetAnimInstance());
        if (AnimInstance)
        {
            Controller->GetWorld()->GetTimerManager().SetTimer(AttackStopTimerHandle, this, &UContinuousAttackBTTaskNode::AttackStop, AttackTime, false);
            AnimInstance->Montage_Play(AttackMontage);
            if (!AnimInstance->OnMontageEnded.Contains(this, FName("OnContinuousAttackMontageEnd")))
            {
                AnimInstance->OnMontageEnded.AddDynamic(this, &UContinuousAttackBTTaskNode::OnContinuousAttackMontageEnd);
            }
            
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UContinuousAttackBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (CanBeAborted && AllowTick)
    {
        bool Res = false;
        switch (AbortionMode)
        {
        case EAbortionMode::Yaw:
            Res = IsStopByYaw();
            break;
        case EAbortionMode::Pitch:
            Res = IsStopByPitch();
            break;
        case EAbortionMode::Both:
            Res = IsStopByBoth();
            break;
        default:
            break;
        }

        if (Res)
        {
            AllowTick = false;
            bNotifyTick = false;
            AttackStop();
        }
    }
}

void UContinuousAttackBTTaskNode::OnContinuousAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == AttackMontage)
    {
        //AnimInstance->OnMontageEnded.Remove(this, FName("OnContinuousAttackMontageEnd"));
        FinishLatentTask(*Controller->GetEnemyBehaviourTreeComponent(), EBTNodeResult::Succeeded);
    }
}

void UContinuousAttackBTTaskNode::AttackStop()
{
    Controller->GetWorld()->GetTimerManager().ClearTimer(AttackStopTimerHandle);
    AnimInstance->Montage_JumpToSection(MontageEndSectionName);
}

bool UContinuousAttackBTTaskNode::IsStopByPitch()
{
    if (AnimInstance)
    {
        if (FMath::Abs(AnimInstance->GetMountPitch()) > AbortionAngle)
        {
            return true;
        }
    }

    return false;
}

bool UContinuousAttackBTTaskNode::IsStopByYaw()
{
    if (AnimInstance)
    {
        if (FMath::Abs(AnimInstance->GetMountYaw()) > AbortionAngle)
        {
            return true;
        }
    }

    return false;
}

bool UContinuousAttackBTTaskNode::IsStopByBoth()
{
    if (IsStopByPitch() || IsStopByYaw())
    {
        return true;
    }

    return false;
}
