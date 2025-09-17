// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpSpawner.generated.h"

#define PICKUP_SIZE_SMALL 2
#define PICKUP_SIZE_MIDDLE 4
#define PICKUP_SIZE_LARGE 8

UENUM()
enum class EPickUpAmount : uint8
{
	Zero UMETA(DisplayName="Zero"),
	Small UMETA(DisplayName="Small"),
    Middle UMETA(DisplayName="Middle"),
    Large UMETA(DisplayName="Large")
};

UENUM()
enum class EPickUpType : uint8
{
	Armour UMETA(DisplayName="Armour"),
	General UMETA(DisplayName="General")
};

UCLASS()
class BLUNDERSGAME_API APickUpSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpSpawner();

	UPROPERTY()
	USceneComponent *SceneComponent;

	UFUNCTION(BlueprintCallable)
	void SetPickUpType(const EPickUpType Type)
	{
		PickUpType = Type;
	}

	UFUNCTION(BlueprintCallable)
	void SetPickUpsAmountToSpawn(const int Amount)
	{
		PickUpsAmountToSpawn = Amount;
	}
	
	void SpawnPickUps(const EPickUpType Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="Resources")
	float SpawnImpulseStrength = 10000.0;
	
	UPROPERTY(BlueprintReadWrite, Category="Resources")
	EPickUpType PickUpType = EPickUpType::Armour;

	UPROPERTY(BlueprintReadWrite, Category="Resources")
	int PickUpsAmountToSpawn = 6;
};
