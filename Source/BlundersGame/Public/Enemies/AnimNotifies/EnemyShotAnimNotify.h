// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "EnemyShotAnimNotify.generated.h"

class AEnemyProjectile;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyShotAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
    UEnemyShotAnimNotify();
    
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Settings")
    FName ShotSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Settings")
    TSubclassOf<AEnemyProjectile> ProjectileType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Settings")
    float SpreadAngle = 0.0;

};
