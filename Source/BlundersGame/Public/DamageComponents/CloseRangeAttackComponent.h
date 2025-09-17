// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"

#include "CloseRangeAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UCloseRangeAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCloseRangeAttackComponent();

	virtual void OnComponentCreated() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent *AttackDetectorSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem *HitEffect;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Activate(bool bReset = false) override;

	virtual void Deactivate() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage settings")
	float Radius = 300.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage settings")
	float Damage = 10.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage settings")
	float AttacksPerMinute = 60.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage settings")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage settings")
	TSubclassOf<AActor> ActorToDamage;

	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
							   AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex,
							   bool bFromSweep,
							   const FHitResult & SweepResult);

	UFUNCTION()
	void ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,
							 AActor* OtherActor,
							 UPrimitiveComponent* OtherComp,
							 int32 OtherBodyIndex);

private:
	UFUNCTION()
	void DealDamage();

	UFUNCTION()
	void OnEnemyStunned(float StunTime);

	UFUNCTION()
	void OnEnemyStunnedOut();

	UPROPERTY()
	AActor *DamageCauser;

	UPROPERTY()
	AController *DamageInstigator;
	
	UPROPERTY()
	AActor *DamagingActor;
	
	UPROPERTY()
	FTimerHandle TH_Damage;

	UPROPERTY()
	FTimerHandle TH_Stun;
};
