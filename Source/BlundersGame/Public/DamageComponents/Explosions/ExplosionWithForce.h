// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageComponents/Explosions/ExplosionBasic.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "ExplosionWithForce.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AExplosionWithForce : public AExplosionBasic
{
	GENERATED_BODY()

public:
	AExplosionWithForce();

	virtual void Destroyed() override;
protected:
	
	virtual void Explosion() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URadialForceComponent *RadialForce;
};
