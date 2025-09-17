// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Enemies/GAS/GASEnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilityTask_EnemyDash.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGASWaitDashOverlapDelegate, AActor*, ActorToDamage);
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UAbilityTask_EnemyDash : public UAbilityTask
{
	GENERATED_BODY()

public:	
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FGASWaitDashOverlapDelegate OnDashOverlap;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
    static UAbilityTask_EnemyDash* EnemyDash(UGameplayAbility *OwningAbility, ACharacter *Character, UPrimitiveComponent *Component, UAnimInstance *AnimInstance,
    	float DashSpeed);

private:	
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UFUNCTION()
	void DashEnd();

	UFUNCTION()
	void OverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY()
	AGASEnemyBase *Enemy;

	UPROPERTY()
	UAnimInstance *AnimInstance;

	UPROPERTY()
	UPrimitiveComponent *EnemyOverlapComponent;

	UPROPERTY()
	UCharacterMovementComponent *EnemyMovementComponent;

	UPROPERTY()
	FTimerHandle DashTimerHandle;

	UPROPERTY()
	float DashSpeed;

	float BrakingFrictionFactorSave;
	float BrakingFrictionSave;
	float GroundFrictionSave;
};
