// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/SimpleFlyToBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

USimpleFlyToBTTaskNode::USimpleFlyToBTTaskNode()
{
    NodeName = "Simple Fly To";
    bNotifyTick = true;
    ObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic);
    QueryParams.bTraceComplex = false;
}

void USimpleFlyToBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
    
    AActor *TargetToFlyTo = Cast<AActor>(BB->GetValueAsObject(ActorToFly.SelectedKeyName));
    const AAIController *Controller = OwnerComp.GetAIOwner();    
    if (TargetToFlyTo && Controller)
    {
        APawn *Pawn = Controller->GetPawn();

        FVector MovementDirection = CalculateFlyingDirection(Pawn, TargetToFlyTo);

        if (MovementDirection.Size() < 0.1f)
        {
            return;
        }

        if (ShouldUseAvoidance)
        {
            const FVector AvoidanceVelocity = BB->GetValueAsVector(AvoidanceVelocityKey.SelectedKeyName);
            MovementDirection += AvoidanceVelocity;
            MovementDirection.Normalize();
        }
        
        Pawn->AddMovementInput(MovementDirection, 1, false);
    }
}

EBTNodeResult::Type USimpleFlyToBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return  EBTNodeResult::InProgress;
}

FVector USimpleFlyToBTTaskNode::CalculateFlyingDirection(const APawn* Pawn, const AActor* Target) const
{
    const FVector PawnLocation = Pawn->GetActorLocation();
    const FVector TargetLocation = Target->GetActorLocation();

    if (AcceptanceRadius > 0.0f)
    {
        const float Distance = UKismetMathLibrary::Vector_Distance(PawnLocation, TargetLocation);

        if (Distance < AcceptanceRadius)
        {
            return FVector::ZeroVector;
        }
    }
        
    FVector MovementDirection = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation).Vector();

    if (TargetHeight > 0.0f)
    {
        FVector TraceEnd = PawnLocation - Pawn->GetActorUpVector() * TargetHeight;
        FHitResult HitResult;
            
        const bool IsDownHit = GetWorld()->LineTraceSingleByObjectType(HitResult, PawnLocation, TraceEnd, ObjectQueryParams, QueryParams);
        if (IsDownHit)
        {
            if (HitResult.Distance > TargetHeight * 0.9 && MovementDirection.Z < 0.0f)
            {
                MovementDirection.Z = 0.0f;
                MovementDirection.Normalize();
            } else
            {
                TraceEnd = PawnLocation + Pawn->GetActorUpVector() * TargetHeight / 2.0f;
                const bool IsUpHit = GetWorld()->LineTraceTestByObjectType(PawnLocation, TraceEnd, ObjectQueryParams, QueryParams);
                if (!IsUpHit)
                {
                    MovementDirection += FVector(0.0f, 0.0f, 0.65f);
                    MovementDirection.Normalize();
                }
            }
        }
    }

    return  MovementDirection;
}
