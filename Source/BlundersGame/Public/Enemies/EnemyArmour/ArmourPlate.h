// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponents/DamageHandlerComponent.h"
#include "GameFramework/Actor.h"
#include "ArmourPlate.generated.h"

UCLASS()
class BLUNDERSGAME_API AArmourPlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmourPlate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *ArmourPlateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USimpleDamageHandlerComponent *DamageHandler;

	UFUNCTION()
	void SetHealth(const float Health);

protected:
	virtual void Destroyed() override;
};
