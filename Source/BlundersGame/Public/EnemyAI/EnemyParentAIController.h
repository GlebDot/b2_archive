// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "Enemies/EnemyParent.h"
#include "EnemyTargetPoint.h"
#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"
#include "EnemyParentAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AEnemyParentAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	AEnemyParentAIController(const FObjectInitializer &ObjectInitializer);
	
	FORCEINLINE UBehaviorTreeComponent *GetEnemyBehaviourTreeComponent() const { return EnemyBehaviourTreeComponent; }

	FORCEINLINE UBlackboardComponent *GetEnemyBlackboardComponent() const { return EnemyBlackboardComponent; }

	FORCEINLINE AEnemyParent *GetEnemyRef() const { return EnemyRef; }

	FORCEINLINE APlayerCharacter *GetPlayerRef() const { return PlayerRef; }

	FORCEINLINE bool IsArenaMasterSet() const {return ArenaMasterRef ? true : false;}

	UFUNCTION()
	void SetArenaRef(AArenaMaster *Arena)
	{
		ArenaMasterRef = Arena;
	}

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION()
	TArray<AEnemyTargetPoint*> &GetTargetPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
    void SetPlayerRef();

	UFUNCTION()
	void EnemyStun(float StunTime);

	UFUNCTION()
	void OnNavLauncherReach();

	virtual void Destroyed() override;
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy AI")
	AEnemyParent *EnemyRef;
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy AI")
	class UEnemyPathFollowingComponent *EnemyPFC;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy AI")
	APlayerCharacter *PlayerRef;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy AI")
	FName TargetPointKey = "TargetPointKey";

	UPROPERTY(EditDefaultsOnly, Category = "Enemy AI")
	FName PlayerKeyName = "PlayerKey";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTreeComponent *EnemyBehaviourTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent *EnemyBlackboardComponent;

private:
	UFUNCTION()
	void EnemyStunOut();
	
	UFUNCTION()
	void OnLanded(const FHitResult &HitInfo);

	UFUNCTION()
    void OnMovementModeChanged(ACharacter* MovedCharacter, EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0);

	UFUNCTION()
	void InitPlayerBBKey();
	
	UPROPERTY()
	AArenaMaster *ArenaMasterRef;

	UPROPERTY()
	FTimerHandle StunTimerHandle;
};
