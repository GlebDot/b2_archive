// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAI/EnemyParentAIController.h"
#include "Animation/AnimMontage.h"
#include "EnemyAnimationAttackBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyAnimationAttackBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()


public:
	UEnemyAnimationAttackBTTaskNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings");
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings");
	UAnimMontage *AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings", meta=(ClampMin="1"));
	int AttacksNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	bool IsTimeoutAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float TimeOutTime = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	FName LoopExitSectionName;
	
protected:
	UPROPERTY()
	UBlackboardComponent *BlackboardComponent;

	UFUNCTION()
	void TimeOut();

private:
	UFUNCTION()
	void OnEndMontage(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	AEnemyParentAIController *Controller;
	
	UPROPERTY()
	UAnimInstance *AnimInstance;

	UPROPERTY()
	int AttackCounter = 0;

	UPROPERTY()
	FTimerHandle TimeOutTimerHandle;
};
