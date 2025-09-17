// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "JumpAttackAnimNotify.generated.h"

class AEnemyParent;

UENUM(BlueprintType)
enum class EJumpAttackType : uint8
{
	ProjectileType UMETA(DisplayName = "ProjectileType"),
    StraightLaunch UMETA(DisplayName = "StraightLaunch")
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UJumpAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()


public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
	float GravityOverride = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
	EJumpAttackType AttackType = EJumpAttackType::ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings", meta=(EditCondition="AttackType==EJumpAttackType::StraightLaunch"))
	float LaunchForce = 2000.0;

	UFUNCTION()
	void ProjectileLaunch(AEnemyParent *Enemy);

	UFUNCTION()
	void StraightLaunch(AEnemyParent *Enemy);
};
