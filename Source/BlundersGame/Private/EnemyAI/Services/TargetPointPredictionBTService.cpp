// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Services/TargetPointPredictionBTService.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UTargetPointPredictionBTService::UTargetPointPredictionBTService()
{
    NodeName = "Target Point Prediction";
    bNotifyTick = true;
}

void UTargetPointPredictionBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
    AActor *Target = Cast<AActor>(BB->GetValueAsObject(TargetActor.SelectedKeyName));

    if (Target)
    {
        //Location Prediction
        FVector PredictedLocation = Target->GetActorLocation();
        const FVector TargetVelocity = Target->GetVelocity() * TargetVelocityInfluence;
        PredictedLocation += TargetVelocity;

        //Navigation Projection
        ANavigationData *NavData = UNavigationSystemV1::GetCurrent(Target->GetWorld())->GetDefaultNavDataInstance();
        FNavLocation NavLocation;
        const bool IsProjected = NavData->ProjectPoint(PredictedLocation, NavLocation, ProjectionExtent);
        if (IsProjected)
        {
            PredictedLocation = NavLocation.Location;            
            BB->SetValueAsVector(Location.SelectedKeyName, PredictedLocation);
        }

        //Debug
        // DrawDebugSphere(Target->GetWorld(), PredictedLocation, 100.0, 12, FColor::Orange, false, DeltaSeconds);
    }
}


