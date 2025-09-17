// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAmmoComponent.generated.h"

class APlayerHUD;

UENUM()
enum class EAmmoRequestType : uint8
{
	Light UMETA(DisplayName="Light"),
	Heavy UMETA(DisplayName="Heavy"),
	Both UMETA(DisplayName="Both"),
	Cigarette UMETA(DisplayName="Cigarette")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UPlayerAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAmmoComponent();

	UFUNCTION(BlueprintCallable, Category="PlayerAmmo")
	void AddLightAmmo(const int Ammo);

	UFUNCTION(BlueprintCallable, Category="PlayerAmmo")
	void AddHeavyAmmo(const int Ammo);

	UFUNCTION(BlueprintCallable, Category="PlayerAmmo")
	void AddCigarette(const int Ammo);

	UFUNCTION(BlueprintCallable, Category="PlayerAmmo")
	bool AmmoRequestCheck(const EAmmoRequestType RequestType, const int RequestAmount);

	UFUNCTION(BlueprintCallable, Category="PlayerAmmo")
	void ReduceAmmo(const EAmmoRequestType AmmoType, const int AmmoAmount);

	virtual void OnComponentCreated() override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	int MaxLightAmmo = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	int MaxHeavyAmmo = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	int MaxCigarettesCount = 20;

private:
	UPROPERTY()
	int CurrentLightAmmo;

	UPROPERTY()
	int CurrentHeavyAmmo;

	UPROPERTY()
	int CurrentCigarettesCount;

	UPROPERTY()
	APlayerHUD *PlayerHUDRef;
};
