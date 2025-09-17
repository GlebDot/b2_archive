// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Enemies/EnemyParent.h"

#include "EnemyLookAtTargetAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyLookAtTargetAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                        float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	float InterpSpeed = 10.0;
private:
	UPROPERTY()
	AEnemyParent *EnemyRef;
};
