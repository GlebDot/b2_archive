// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/EnemyAnimationAttackBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"

UEnemyAnimationAttackBTTaskNode::UEnemyAnimationAttackBTTaskNode()
{
    NodeName = "Animation Attack";
    ForceInstancing(true);
}

EBTNodeResult::Type UEnemyAnimationAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());
    if (Controller != nullptr && Controller->GetEnemyRef() != nullptr)
    {
        BlackboardComponent = Controller->GetBlackboardComponent();

        //Pick Attack Target and Set in Enemy class
        AActor *AttackTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(AttackTargetKey.SelectedKeyName));
        Controller->GetEnemyRef()->SetCurrentTarget(AttackTarget);
        

        //Get Animation Instance
        AnimInstance = Controller->GetEnemyRef()->GetMesh()->GetAnimInstance();
        
        if (AnimInstance)
        {         
            //Playing Attack montage
            AnimInstance->Montage_Play(AttackMontage);
            AttackCounter++;
            if (IsTimeoutAttack)
            {
                Controller->GetWorld()->GetTimerManager().SetTimer(TimeOutTimerHandle, this, &UEnemyAnimationAttackBTTaskNode::TimeOut, TimeOutTime, false);
            }
            AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UEnemyAnimationAttackBTTaskNode::OnEndMontage);            
            
            return EBTNodeResult::InProgress;
        }        
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}

void UEnemyAnimationAttackBTTaskNode::TimeOut()
{
    if (AnimInstance)
    {
        AnimInstance->Montage_JumpToSection(LoopExitSectionName);
    }
}

void UEnemyAnimationAttackBTTaskNode::OnEndMontage(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == AttackMontage)
    {
        if (AttackCounter == AttacksNumber)
        {
            AttackCounter = 0;
            AnimInstance->OnMontageEnded.Remove(this, FName("OnEndMontage"));
            FinishLatentTask(*Controller->GetEnemyBehaviourTreeComponent(), EBTNodeResult::Succeeded);
        } else
        {
            AnimInstance->Montage_Play(AttackMontage);
            AttackCounter++;
        }
    }
}
