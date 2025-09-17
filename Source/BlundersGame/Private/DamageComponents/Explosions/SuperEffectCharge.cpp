// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/Explosions/SuperEffectCharge.h"

// Sets default values
ASuperEffectCharge::ASuperEffectCharge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	ExplosionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExplosionMesh"));

	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}

	ExplosionMesh->SetupAttachment(SceneComponent);
}

