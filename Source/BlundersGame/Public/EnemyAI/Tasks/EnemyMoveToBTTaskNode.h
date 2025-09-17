// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "BlundersGame/Public/EnemyAI/EnemyParentAIController.h"
#include "EnemyMoveToBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyMoveToBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEnemyMoveToBTTaskNode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Base")
	FBlackboardKeySelector ActorToMoveTo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Base")
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Base")
	TSubclassOf<UNavigationQueryFilter> NavFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Base")
	bool StopOnOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|StopTimer")
	bool IsStopTimer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|StopTimer")
	float StopTime = 5.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|StopTimer")
	bool IsSuccessAfterStop = false;

	UPROPERTY()
	AActor *Target;

	UPROPERTY()
	AEnemyParentAIController *Controller;

	UFUNCTION()
	void StopTimer(UBehaviorTreeComponent* OwnerComp);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FHitResult HitInfo;
	FVector TraceStart;
	FVector TraceEnd;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams QueryParams;

	FTimerHandle StopTimerHandle;
	FTimerDelegate StopTimerDelegate;
};
