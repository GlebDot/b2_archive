// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Services/AvoidanceVelCalcBTService.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "Kismet/KismetMathLibrary.h"

UAvoidanceVelCalcBTService::UAvoidanceVelCalcBTService()
{
    NodeName = "Calculate Avoidance Velocity";
    bNotifyTick = true;

    ObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn);
    QueryParams.bTraceComplex = false;
    Shape = FCollisionShape::MakeSphere(TraceSphereRadius);
}

void UAvoidanceVelCalcBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
    if (OwnerComp.GetAIOwner() && BB)
    {
        APawn *Pawn = OwnerComp.GetAIOwner()->GetPawn();

        if (Pawn)
        {
            FVector VelocityForwardVector = Pawn->GetVelocity();
            VelocityForwardVector.Normalize();         
            FVector VelocityUpVector = FVector::UpVector;
            FVector VelocityRightVector = FVector::RightVector;
    
            FVector::CreateOrthonormalBasis(VelocityUpVector, VelocityRightVector, VelocityForwardVector);

            FVector TraceDirection = VelocityForwardVector.RotateAngleAxis(VisibilityAngle, VelocityUpVector);

            TArray<FHitResult> HitResults;
            TArray<FHitResult> AllHitResults;
            const FVector TraceStart = Pawn->GetActorLocation();
            FVector TraceEnd = TraceStart + VelocityForwardVector * VisibilityDistance;
            
            for (int i = 0; i < 5; i++)
            {              
                Pawn->GetWorld()->SweepMultiByObjectType(HitResults, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, QueryParams);
                AllHitResults.Append(HitResults);
                HitResults.Empty();

                TraceDirection = TraceDirection.RotateAngleAxis(90.0f, VelocityForwardVector);
                TraceEnd = TraceStart + TraceDirection * VisibilityDistance;
            }

            FVector AvoidanceVelocity = FVector::ZeroVector;
            for (auto &HitResult : AllHitResults)
            {
                if (HitResult.GetActor() != nullptr && HitResult.GetActor() != Pawn)
                {
                    if (HitResult.GetActor()->IsA(EnemyTypeToAvoid))
                    {
                        AActor *Other = HitResult.GetActor();
                        FVector OtherVelocity = Other->GetVelocity();
                        OtherVelocity.Normalize();
                        const float DotProduct = FVector::DotProduct(VelocityForwardVector, OtherVelocity);
                        if (DotProduct > 0.1)
                        {
                            AvoidanceVelocity += UKismetMathLibrary::FindLookAtRotation(Other->GetActorLocation(), TraceStart).Vector();
                        }
                    }
                }
            }

            AvoidanceVelocity.Normalize();
            AvoidanceVelocity = AvoidanceVelocity * AvoidanceStrength;

            BB->SetValueAsVector(AvoidanceVelocityKey.SelectedKeyName, AvoidanceVelocity);
        }
    }
}
