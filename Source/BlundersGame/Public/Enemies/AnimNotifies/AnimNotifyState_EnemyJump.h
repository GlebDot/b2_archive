// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "AnimNotifyState_EnemyJump.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UAnimNotifyState_EnemyJump : public UAnimNotifyState
{
	GENERATED_BODY()


public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump Settings", meta=(DisplayPriority = "0"))
	float JumpVelocity = 500.0;

	/**
	 * If set to true enemy will be hanging in air during the whole notify state
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump Settings", meta=(DisplayPriority = "1"))
	bool IsEnemyHanging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump Settings", meta=(DisplayPriority = "2", EditCondition = "IsEnemyHanging"))
	float GravityRestoreValue;
};
