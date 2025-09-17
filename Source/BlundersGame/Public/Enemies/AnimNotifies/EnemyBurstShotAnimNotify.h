// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/AnimNotifies/EnemyShotAnimNotify.h"
#include "EnemyBurstShotAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyBurstShotAnimNotify : public UEnemyShotAnimNotify
{
	GENERATED_BODY()
	
public:
	UEnemyBurstShotAnimNotify() {}

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Settings")
	FVector2D MinMaxProjectiles = FVector2D(7, 10);
};
