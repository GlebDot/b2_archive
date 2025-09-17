// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBasicAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyBasicAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
	AActor *GetCurrentEnemyTarget() const;

	UFUNCTION()
	void SetNormalPitch(float Pitch);
	
	UFUNCTION()
	float GetNormalPitch() const;

	UFUNCTION()
    void SetInterpSpeedCalculation(float InterpSpeed);

	UFUNCTION()
	void SetTargetPitchCalculation(bool IsCalculating);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Animation|Basics")
	bool IsFalling;
	
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Animation|Basics")
	float Speed;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Animation|Basics")
	float NormalPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Animation|Basics")
	class AEnemyParent *EnemyRef;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Animation|Basics")
	float InerpSpeedCalculation;

	UFUNCTION()
	void CalculateTargetPitch();

private:
	UFUNCTION()
	void CalculatingTargetPitchSwitch(bool IsCalculating);

	UPROPERTY()
	FTimerHandle PitchCalculationTimerHandle;
	
	UPROPERTY()
	bool IsCalculatingTargetPitch = false;
		
public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
