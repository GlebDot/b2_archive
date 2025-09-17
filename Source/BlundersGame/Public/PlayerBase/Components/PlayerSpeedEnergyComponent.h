// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSpeedEnergyComponent.generated.h"

class APlayerCharacter;

UENUM()
enum class ESpeedEnergyUsageType : uint8
{
	Health UMETA(DisplayName="Health"),
	Ammo UMETA(DisplayName="Ammo")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UPlayerSpeedEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSpeedEnergyComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentCreated() override;

	UFUNCTION()
	void SetPlayerRef(APlayerCharacter *PlayerCharacter);

	UFUNCTION()
	void UseEnergy(const ESpeedEnergyUsageType UsageType);

	UFUNCTION()
	void AddEnergy(const float EnergyAmount);

	UPROPERTY()
	UStaticMeshComponent *SpeedEnergyMesh;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Energy")
	float MaxEnergyPerCell = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Energy")
	int MaxEnergyCells = 3;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Energy")
	float EnergyAccumulationSpeed = 100.0f;
	
private:
	UPROPERTY()
	float CurrentEnergyPerCell = 0.0;

	UPROPERTY()
	float MaxMovementSpeed = 1.0;

	UPROPERTY()
	int CurrentEnergyCells = 0;

	UPROPERTY()
	APlayerCharacter *Player;
};
