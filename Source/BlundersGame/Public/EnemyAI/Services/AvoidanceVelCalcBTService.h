// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AvoidanceVelCalcBTService.generated.h"

class AEnemyParent;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UAvoidanceVelCalcBTService : public UBTService
{
	GENERATED_BODY()

public:
    UAvoidanceVelCalcBTService();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Avoidance Velocity")
    FBlackboardKeySelector AvoidanceVelocityKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Avoidance Velocity")
    float AvoidanceStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Avoidance Velocity")
    float VisibilityAngle = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Avoidance Velocity")
    float VisibilityDistance = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Avoidance Velocity")
    float TraceSphereRadius = 75.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Avoidance Velocity")
    TSubclassOf<AEnemyParent> EnemyTypeToAvoid;

private:
    FCollisionQueryParams QueryParams;
    FCollisionObjectQueryParams ObjectQueryParams;
    FCollisionShape Shape;
};
