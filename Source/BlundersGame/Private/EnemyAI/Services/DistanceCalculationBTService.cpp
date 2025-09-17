// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Services/DistanceCalculationBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/PlayerCharacter.h"

UDistanceCalculationBTService::UDistanceCalculationBTService()
{
    NodeName = "Calculate Distance and Visibility";
    bNotifyTick = true;
    ForceInstancing(false);
}

void UDistanceCalculationBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    

    float Distance;
    UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
    AActor *TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
    APawn *OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (TargetActor)
    {
        Distance = UKismetMathLibrary::Vector_Distance(TargetActor->GetActorLocation(), OwnerPawn->GetActorLocation());
    } else
    {
        Distance = 10000000.0f;
    }

    const bool CanSeeTarget = IsTargetVisible(OwnerPawn, TargetActor);
    

    BB->SetValueAsFloat(TargetDistanceKey.SelectedKeyName, Distance);
    BB->SetValueAsBool(VisibilityKey.SelectedKeyName, CanSeeTarget);
}

bool UDistanceCalculationBTService::IsTargetVisible(AActor* Source, AActor* Target) const
{
    FVector SourceLocation = Source->GetActorLocation();
    const FVector TargetLocation = Target->GetActorLocation();

    const FRotator SightRotation = UKismetMathLibrary::FindLookAtRotation(SourceLocation, TargetLocation);

    SourceLocation += SightRotation.Vector() * Source->GetSimpleCollisionRadius() * 2;
    FHitResult Hit;
    Source->GetWorld()->LineTraceSingleByChannel(Hit, SourceLocation, TargetLocation, ECollisionChannel::ECC_Camera);

    if(Hit.GetActor())
    {
        APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(Hit.GetActor());
        if (PlayerCharacter)
        {
            return true;
        }
    }

    return false;
}
