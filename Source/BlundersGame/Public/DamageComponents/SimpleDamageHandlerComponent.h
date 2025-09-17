// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleDamageHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API USimpleDamageHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleDamageHandlerComponent();

	UFUNCTION()
	void SetMaxHealth(const float Health);

	UFUNCTION(BlueprintCallable, Category = "Damage handling")
    bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Damage handling")
    float GetHealthPercent() const;

	virtual void OnComponentCreated() override;

protected:
	UFUNCTION()
    virtual void TakeDamage(AActor* DamagedActor,
							float Damage,
							const class UDamageType* DamageType,
							class AController* InstigatedBy,
							AActor* DamageCauser);

	UFUNCTION()
    virtual void ReduceHealth(float HealthReducingAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	int MaxHealth = 100; UMETA(DisplayPriority="1")

	UPROPERTY()
	int CurrentHealth;

	UPROPERTY()
	bool bIsDead = false;
};
