// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseCharacter.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"
#include "EnemyParent.generated.h"

class UBehaviorTree;
class AEnemyProjectile;
class AEnemyTargetPoint;


UCLASS()
class BLUNDERSGAME_API AEnemyParent : public AEnemyBaseCharacter
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	AActor *GetCurrentTarget() const
	{
		return CurrentTarget;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetCurrentTarget(AActor *Target)
	{
		CurrentTarget = Target;
	}

	UFUNCTION()
	void RegisterArena(AArenaMaster *Arena);
	
	// Sets default values for this character's properties
	AEnemyParent();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

	virtual void Reset() override;

	virtual void Death_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetRotationLookAtTarget(float RotationInterpSpeed);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	APlayerCharacter *GetPlayerRef() const
	{
		return PlayerRef;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	AArenaMaster *GetArenaRef() const
	{
		return ArenaMasterRef;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy|Weight")
	float GetEnemyWeight() const
	{
		return EnemyWeight;
	}

	UFUNCTION(BlueprintCallable, Category="Enemy|Weight")
	void SetCurrentTargetPoint(AEnemyTargetPoint *Point);

	UFUNCTION(BlueprintCallable, Category = "Enemy|Tickets")
	void ReceiveTickets(int Tickets);

	UFUNCTION(BlueprintCallable, Category = "Enemy|Tickets")
    void ReturnTicketsToArena();

	UFUNCTION()
    void RebindStopEffect();

	UFUNCTION()
	virtual void OnStopEffect(float StunTime);
	
protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	UFUNCTION()
	void ArenaMasterInfoCleanUp();
	
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Settings|Properties")
	float EnemyWeight = 10.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Settings|Properties")
	UAnimMontage *StunMontage;

	FDelegateHandle StopEffectDelegate;

private:
	
	UPROPERTY()
	AActor *CurrentTarget;

	UPROPERTY()
	AArenaMaster *ArenaMasterRef;

	UPROPERTY()
	int EnemyTickets = 0;

	UPROPERTY()
	AEnemyTargetPoint *CurrentTargetPoint;

	UPROPERTY()
	FTimerHandle TH_StunMontageEnd;
	
	UFUNCTION()
	void StunMontageEnd();
};
