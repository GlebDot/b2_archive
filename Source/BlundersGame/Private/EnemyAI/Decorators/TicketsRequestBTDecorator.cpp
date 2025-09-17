// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Decorators/TicketsRequestBTDecorator.h"
#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"
#include "EnemyAI/EnemyParentAIController.h"

bool UTicketsRequestBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    AEnemyParentAIController *Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());
    if (Controller)
    {
        AEnemyParent *Enemy = Controller->GetEnemyRef();

        if (Enemy->GetArenaRef()->ArenaTicketsCheck(TicketsToDecrease))
        {
            Enemy->ReceiveTickets(TicketsToDecrease);
            return true;
        }

        return false;
    }

    return false;
}

UTicketsRequestBTDecorator::UTicketsRequestBTDecorator()
{
    NodeName = "Tickets Request";
}
