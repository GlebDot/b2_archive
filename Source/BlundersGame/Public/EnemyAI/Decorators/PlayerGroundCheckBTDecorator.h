// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "PlayerGroundCheckBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerGroundCheckBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
    UPlayerGroundCheckBTDecorator();
protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, ::uint8* NodeMemory) const override;

    UPROPERTY(EditAnywhere, Category="Node")
    FBlackboardKeySelector Player;
};
