// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/TicketsReturnBTTaskNode.h"
#include "BlundersGame/Public/EnemyAI/EnemyParentAIController.h"

UTicketsReturnBTTaskNode::UTicketsReturnBTTaskNode()
{
    NodeName = "Return Tickets";
    ForceInstancing(true);
}

EBTNodeResult::Type UTicketsReturnBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());
    if (Controller)
    {
        BTComp = &OwnerComp;

        Controller->GetWorld()->GetTimerManager().SetTimer(NodeEndDelayHandle, this, &UTicketsReturnBTTaskNode::NodeEnd, WaitTime, false);

        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UTicketsReturnBTTaskNode::NodeEnd()
{
    Controller->GetWorld()->GetTimerManager().ClearTimer(NodeEndDelayHandle);
    Controller->GetEnemyRef()->ReturnTicketsToArena();

    FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
}
