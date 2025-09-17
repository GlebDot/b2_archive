// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PickAttackTargetBTTaskNode.generated.h"

UENUM(BlueprintType)
enum class EAttackTargetType : uint8
{
	Player UMETA(DisplayName = "Player"),
	LevelController UMETA(DisplayName = "Level Controller")
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPickAttackTargetBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()


public:
	UPickAttackTargetBTTaskNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	EAttackTargetType TargetToSearch;

protected:

	UPROPERTY()
	AActor *TargetActor;

	void FindPlayer();

	
};
