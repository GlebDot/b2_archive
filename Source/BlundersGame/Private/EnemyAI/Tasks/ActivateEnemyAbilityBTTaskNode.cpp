// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/ActivateEnemyAbilityBTTaskNode.h"
#include "BlundersGame/Public/Enemies/GAS/GASEnemyBase.h"
#include "EnemyAI/EnemyParentAIController.h"
#include "AbilitySystemComponent.h"

UActivateEnemyAbilityBTTaskNode::UActivateEnemyAbilityBTTaskNode()
{
    NodeName = "ActivateEnemyAbility";
    ForceInstancing(true);
}

EBTNodeResult::Type UActivateEnemyAbilityBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyParentAIController *Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());
    if (Controller)
    {
        Enemy = Cast<AGASEnemyBase>(Controller->GetPawn());
        if (Enemy)
        {
            Enemy->ActivateAbility(AbilityType);
            FOnGameplayAbilityEnded AbilityEnded;
            Enemy->GetAbilitySystemComponent()->OnAbilityEnded.AddUObject(this, &UActivateEnemyAbilityBTTaskNode::AbilityEnded, &OwnerComp);
            RemainingAttacks = AttacksNumber;
            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UActivateEnemyAbilityBTTaskNode::AbilityEnded(const FAbilityEndedData& AbilityData, UBehaviorTreeComponent* OwnerComp)
{
    RemainingAttacks--;
    if (RemainingAttacks > 0)
    {
        Enemy->ActivateAbility(AbilityType);  
    } else
    {
        Enemy->GetAbilitySystemComponent()->OnAbilityEnded.RemoveAll(this);
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    }
}
