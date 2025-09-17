// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Enemies/Misc/EnemyAttacksDataAsset.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ActivateEnemyAbilityBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UActivateEnemyAbilityBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:	
	UActivateEnemyAbilityBTTaskNode();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Ability Settings")
	EEnemyAbilityType AbilityType;

	UPROPERTY(EditAnywhere, Category = "Ability Settings", meta=(ClampMin="1"));
	int AttacksNumber = 1;

	UFUNCTION()
	void AbilityEnded(const FAbilityEndedData& AbilityData, UBehaviorTreeComponent* OwnerComp);

private:
	int RemainingAttacks;

	UPROPERTY()
	class AGASEnemyBase *Enemy; 
};
