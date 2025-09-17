// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Enemies/GAS/GASEnemyBase.h"

#include "AbilityTask_JumpCorrection.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGASWaitLandedDelegate, const FVector&, TargetLocation);

UCLASS()
class BLUNDERSGAME_API UAbilityTask_JumpCorrection : public UAbilityTask
{
	GENERATED_BODY()

public:
    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FGASWaitLandedDelegate WaitOnLanded;

    UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
    static UAbilityTask_JumpCorrection* JumpCorrection(UGameplayAbility *OwningAbility, ACharacter *Character, UAnimInstance *AnimInstance);

private:
    virtual void OnDestroy(bool bInOwnerFinished) override;

    UFUNCTION()
    void TrajectoryCorrection();

    UFUNCTION()
    void LandedCallback(const FHitResult& Hit);

    UPROPERTY()
    AGASEnemyBase *ControlledCharacter;

    UPROPERTY()
    AActor *TargetActor;

    UPROPERTY()
    UAnimInstance *AnimInstance;

    UPROPERTY()
    float MontageLength;

    UPROPERTY()
    FVector InitialVelocity;

    UPROPERTY()
    FTimerHandle TrajectoryCorrectionTimerHandle;
};
