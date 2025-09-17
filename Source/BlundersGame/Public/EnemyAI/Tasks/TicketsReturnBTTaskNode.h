// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TicketsReturnBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UTicketsReturnBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTicketsReturnBTTaskNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void NodeEnd();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node");
	float WaitTime = 3.0;

private:
	UPROPERTY()
	class AEnemyParentAIController *Controller;

	UPROPERTY()
	UBehaviorTreeComponent *BTComp;
	
	UPROPERTY()
	FTimerHandle NodeEndDelayHandle;
};
