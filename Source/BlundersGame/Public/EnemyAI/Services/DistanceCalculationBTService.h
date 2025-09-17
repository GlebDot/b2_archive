// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "DistanceCalculationBTService.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UDistanceCalculationBTService : public UBTService
{
	GENERATED_BODY()

public:
	UDistanceCalculationBTService();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	FBlackboardKeySelector TargetDistanceKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	FBlackboardKeySelector VisibilityKey;

private:
	UFUNCTION()
	bool IsTargetVisible(AActor *Source, AActor *Target) const;
};
