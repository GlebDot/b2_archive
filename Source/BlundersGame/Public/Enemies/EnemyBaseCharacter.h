// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DamageComponents/Damagable.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "PlayerBase/ResourcePickUps/PickUpSpawner.h"
#include "EnemyBaseCharacter.generated.h"

class UDamageHandlerComponent;
class APlayerCharacter;
class UEnemyBaseInfoDataAsset;

DECLARE_DELEGATE_OneParam(FOnEnemyReceivedDamage, float);

UENUM()
enum class EEnemyJumpResponse : uint8
{
	NoJump UMETA(DisplayName="NoJump"),
	Jump UMETA(DisplayName="Jump"),
	Trap UMETA(DisplayName="Trap"),
};


UCLASS()
class BLUNDERSGAME_API AEnemyBaseCharacter : public ACharacter, public IDamagable
{
	GENERATED_BODY()

public:
	FOnEnemyReceivedDamage OnEnemyReceivedDamage;
	
	// Sets default values for this character's properties
	AEnemyBaseCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void ReceiveDamage(const FDamageEffects& DamageEffects) override;

	UFUNCTION()
    virtual void OnActorDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy")
    void Death();
	virtual void Death_Implementation();

	UFUNCTION(BlueprintCallable, Category="Enemy")
    virtual EEnemyJumpResponse PlayerJumpedOnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings|Properties");
	UBehaviorTree* EnemyBehaviourTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings|Properties");
	UEnemyBaseInfoDataAsset *BaseInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UDamageHandlerComponent* DamageHandler;

	FORCEINLINE void SetIsKilledBySuper(const bool value) {IsKilledBySuper = value;}
	FORCEINLINE bool GetIsKilledBySuper() const {return IsKilledBySuper;}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	class APlayerHUD *PlayerHUD;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCharacterMovementComponent *MovementComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Settings|Properties")
	EEnemyJumpResponse EnemyJumpResponse;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Settings|Properties")
	EPickUpType PickUpType;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Settings|Properties")
	EPickUpAmount PickUpAmount;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Info")
	APlayerCharacter *PlayerRef;

private:
	UPROPERTY()
	bool IsHeatColorSwitched = false;

	UPROPERTY()
	bool IsKilledBySuper = false;
};
