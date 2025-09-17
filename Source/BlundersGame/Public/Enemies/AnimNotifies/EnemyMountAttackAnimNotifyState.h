// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Enemies/EnemyParent.h"

#include "EnemyMountAttackAnimNotifyState.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Laser UMETA(DisplayName = "Laser"),
	Gatling UMETA(DisplayName = "Gatling")
};

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyMountAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()


public:

	UEnemyMountAttackAnimNotifyState();
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime) override;


	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Anim State")
	float InterpolationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|Anim State")
	FName ShotSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|AttackSettings")
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|AttackSettings")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|AttackSettings")
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify|AttackSettings")
	TSubclassOf<UDamageType> DamageType;

private:
	void LaserAttack(const UWorld *World, const FVector &StartLocation, const FRotator &SocketRotation);

	FCollisionObjectQueryParams ObjectParams;

	FCollisionShape LaserShape;

	FCollisionQueryParams QueryParams;
};
