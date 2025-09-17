// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponents/DamageHandlerComponent.h"
#include "ArmourDamageHandlerComponent.generated.h"

DECLARE_DELEGATE(FOnPlayerHealthDecreased)

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UArmourDamageHandlerComponent : public USimpleDamageHandlerComponent
{
	GENERATED_BODY()

public:
	FOnPlayerHealthDecreased OnPlayerHealthDecreased;
	
	UArmourDamageHandlerComponent();

	virtual void OnComponentCreated() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Damage handling")
	float GetArmourPercent();

	UFUNCTION(BlueprintCallable, Category="Damage handling")
	void AbsoluteDamage(const int Damage);

	virtual void ReduceHealth(float HealthReducingAmount) override;

	void ReduceArmour(int ArmourReducing);

	void AddArmourFromPickUp(const int Armour);

	void AddHealthFromPickUp(const int Health);

	int GetArmour() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UStaticMeshComponent *ArmourDisplayMesh;

	UPROPERTY()
	UStaticMeshComponent *HealthAlertMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resources")
	int MaxArmour = 100; UMETA(DisplayPriority="1")

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resources")
	float ImmortalTime = 0.1f; UMETA(DisplayPriority="2")

private:

	UPROPERTY()
	int CurrentArmour;

	UPROPERTY()
	bool IsImmortal = false;

	UPROPERTY()
	FTimerHandle TH_Immortal;

	UFUNCTION()
	void ResetImmortal();
};
