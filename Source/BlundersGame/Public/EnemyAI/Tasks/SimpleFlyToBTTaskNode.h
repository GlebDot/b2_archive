// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SimpleFlyToBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API USimpleFlyToBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USimpleFlyToBTTaskNode();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fly Settings")
	FBlackboardKeySelector ActorToFly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fly Settings")
	float AcceptanceRadius = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fly Settings")
	float TargetHeight = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fly Settings", Meta = (AdvancedDisplay))
	bool ShouldUseAvoidance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fly Settings", Meta = (AdvancedDisplay, EditCondition="ShouldUseAvoidance"))
	FBlackboardKeySelector AvoidanceVelocityKey;

	UFUNCTION()
	FVector CalculateFlyingDirection(const APawn* Pawn, const AActor *Target) const;

private:	
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams QueryParams;
};
