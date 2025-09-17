// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TargetPointPredictionBTService.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UTargetPointPredictionBTService : public UBTService
{
	GENERATED_BODY()
public:
    UTargetPointPredictionBTService();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FBlackboardKeySelector TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FBlackboardKeySelector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FVector ProjectionExtent = FVector(1024.0f, 1024.0f, 1024.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    float TargetVelocityInfluence = 1.0f;
};
