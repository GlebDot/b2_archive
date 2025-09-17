// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAI/EnemyParentAIController.h"

#include "ContinuousAttackBTTaskNode.generated.h"

class UEnemyMountAnimInstance;

UENUM(BlueprintType)
enum class EAbortionMode : uint8
{
    Yaw UMETA(DisplayName = "Yaw"),
    Pitch UMETA(DisplayNems = "Pitch"),
    Both UMETA(DisplayName = "Both")
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UContinuousAttackBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()


public:
    UContinuousAttackBTTaskNode();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:

    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    UAnimMontage *AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    float AttackTime = 3.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FName MontageEndSectionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    FBlackboardKeySelector AttackTargetKey;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Abortion")
    bool CanBeAborted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Abortion")
    EAbortionMode AbortionMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Abortion")
    float AbortionAngle;

private:
    UFUNCTION()
    void OnContinuousAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
    
    UFUNCTION()
    void AttackStop();

    FOnMontageEnded EnemyContinuousAttackEndDelegate;
    
    UPROPERTY()
    FTimerHandle AttackStopTimerHandle;

    UPROPERTY()
    AEnemyParentAIController *Controller;

    UPROPERTY()
    UEnemyMountAnimInstance *AnimInstance;

    //I don't know how to manipulate ticking state with default functionality
    //For some reasons if I'm instancing node the variable bTickNotify doesn't mean anything and
    //If I change it during execution of this node (in ExecuteTak or during TickTask) it won't work
    //For now I don't know why it happening and what is why I making this artificial way of controlling Tick
    //With custom bool variable. If it's set to false the tick won't work, if it set to true tick will work.
    UPROPERTY()
    bool AllowTick = false;

    UFUNCTION()
    bool IsStopByPitch();

    UFUNCTION()
    bool IsStopByYaw();

    UFUNCTION()
    bool IsStopByBoth();
};
