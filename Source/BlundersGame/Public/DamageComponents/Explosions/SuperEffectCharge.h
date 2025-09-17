// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuperEffectCharge.generated.h"

UCLASS()
class BLUNDERSGAME_API ASuperEffectCharge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuperEffectCharge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent *SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *ExplosionMesh;

};
