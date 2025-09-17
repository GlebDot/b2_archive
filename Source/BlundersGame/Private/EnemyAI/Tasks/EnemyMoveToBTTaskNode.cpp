// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/EnemyMoveToBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAI/EnemyParentAIController.h"

UEnemyMoveToBTTaskNode::UEnemyMoveToBTTaskNode()
{
    NodeName = "Enemy Move To";
    bNotifyTick = true;
    ObjectParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic);
    QueryParams.bTraceComplex = false;
    ForceInstancing(true);
}

void UEnemyMoveToBTTaskNode::StopTimer(UBehaviorTreeComponent* OwnerComp)
{
    GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
    Controller->StopMovement();
    
    if (IsSuccessAfterStop)
    {
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    } else
    {
        FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
    }
}

EBTNodeResult::Type UEnemyMoveToBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());
    
    if (Controller)
    {
        UBlackboardComponent *BlackboardComponent = Controller->GetBlackboardComponent();
        Target = Cast<AActor>(BlackboardComponent->GetValueAsObject(ActorToMoveTo.SelectedKeyName));

        if (!Target)
        {
            return EBTNodeResult::Failed;
        }

        if (IsStopTimer)
        {
            StopTimerDelegate.BindUFunction(this, FName("StopTimer"), &OwnerComp);
            GetWorld()->GetTimerManager().SetTimer(StopTimerHandle, StopTimerDelegate, StopTime, false);
        }

        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UEnemyMoveToBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (Target)
    {

        TraceStart = Target->GetActorLocation();
        TraceEnd = TraceStart + Target->GetActorUpVector() * -10000.0;
        GetWorld()->LineTraceSingleByObjectType(HitInfo,
                                                TraceStart,
                                                TraceEnd,
                                                ObjectParams,
                                                QueryParams);
        
        EPathFollowingRequestResult::Type Result = Controller->MoveToLocation(HitInfo.Location,
                                                                              AcceptanceRadius,
                                                                              StopOnOverlap,
                                                                              true,
                                                                              true,
                                                                              true,
                                                                              NavFilter,
                                                                              true);

        switch(Result)
        {
        case EPathFollowingRequestResult::Failed:
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            break;
        case EPathFollowingRequestResult::RequestSuccessful:
            break;
        case EPathFollowingRequestResult::AlreadyAtGoal:
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            break;
        }
    }
}
