// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TicketsRequestBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UTicketsRequestBTDecorator : public UBTDecorator
{
	GENERATED_BODY()


protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tickets")
	int TicketsToDecrease;
	
public:
	UTicketsRequestBTDecorator();
};
