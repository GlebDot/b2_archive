// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damagable.h"
#include "SimpleDamageHandlerComponent.h"
#include "StatusEffects/StatusEffectBase.h"
#include "DamageHandlerComponent.generated.h"

struct FDamageEffects;
class AEnemyBaseCharacter;
class AWeaponProjectile;
class ASuperEffectCharge;

DECLARE_MULTICAST_DELEGATE_OneParam(FStopEffectTresholdAchieved, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UDamageHandlerComponent : public USimpleDamageHandlerComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageHandlerComponent();

	FStopEffectTresholdAchieved OnStopEffectAchieved;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

    void HandleDamage(const FDamageEffects& InputDamage);

	UFUNCTION(BlueprintCallable, Category = "Effects conditions")
    int GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION()
	void ForceEnemyStun();

	UFUNCTION()
	EStatusEffectType GetCurrentStatusEffectType() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TakeDamage(AActor* DamagedActor,
					float Damage,
					const class UDamageType* DamageType,
					class AController* InstigatedBy,
					AActor* DamageCauser) override;

	void TrySetNewStatusEffect(const FDamageEffects& Effects);

	UFUNCTION()
	void SetCurrentStatusEffect(const EStatusEffectType& NewStatusEffectType);

	virtual void ReduceHealth(float HealthReducingAmount) override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Resources")
	float StunTime = 0.0f;

protected:	
	UPROPERTY()
	UStatusEffectBase *CurrentStatusEffect;

	UPROPERTY()
	AEnemyBaseCharacter *EnemyRef;

private:
	UPROPERTY()
	int TotalDamage = 0;

	UPROPERTY()
	const UDamageType *PreviousDamageType;
};
