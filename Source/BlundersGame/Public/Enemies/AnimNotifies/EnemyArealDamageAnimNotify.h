// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyArealDamageAnimNotify.generated.h"

UENUM()
enum class EDamageOffsetDirection : uint8
{
	Up UMETA(DisplayName = "Up"),
	Forward UMETA(DisplayName = "Forward"),
	Right UMETA(DisplayName = "Right")
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyArealDamageAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Settings")
	float BaseDamage = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Settings")
	float DamageRadius = 300.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Settings")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Settings")
	EDamageOffsetDirection OffsetDirection = EDamageOffsetDirection::Up;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Settings")
	float DamageOffset;
};
