// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PickEnemyTargetPointBTTaskNode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETargetPointPickClassification : uint8
{
	MinWeight UMETA(DisplayName = "MinWeight"),
    MaxHidden UMETA(DisplayName = "MaxHidden"),
    MinHidden UMETA(DisplayName = "MinHidden")
};

UCLASS()
class BLUNDERSGAME_API UPickEnemyTargetPointBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()


public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPickEnemyTargetPointBTTaskNode();

	UPROPERTY(EditAnywhere, Category = "Node")
	FBlackboardKeySelector TargetPointKey;

	UPROPERTY(EditAnywhere, Category = "Node")
	ETargetPointPickClassification PickClassification = ETargetPointPickClassification::MinWeight;
};
